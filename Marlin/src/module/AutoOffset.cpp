#include "AutoOffset.h"



#if ENABLED(USE_AUTOZ_TOOL_2)

/*
* Function Name: 	getStr(float f)
* Purpose: 			  将浮点转成字符串（注意：此处不使用ftoa()或%f，是因为部分Marlin版本BUG，会导致浮点转字符失败）
* Params :        (float)f 需要转换的浮点值
* Return: 			  (char*)  转换后的字符串
* Attention:      重载调用次数不要超过16
*/
char *getStr(float f)
{
  static char str[16][16];
  static int index = 0;

  memset(str[index % 16], '\0', 16);
  sprintf(str[index % 16], (f >= 0 ? "+%d.%03d" : "-%d.%03d"), (int)fabs(f),  ((int)(fabs(f) * 1000)) % 1000);

  return str[index++ % 16];
}

/*
* Function Name: 	ckGpioIsInited(int pin)
* Purpose: 			  检测给定pin是否已经初始化过，避免对clk重复初始化而导致时序错乱
* Params :        (int)pin 待检测的pin
* Return: 			  (bool) true=pin已初始化过；false=pin未经过初始化。
*/
bool HX711::ckGpioIsInited(int pin)
{
  static int pinList[32] = {0};
  FOR_LOOP_TIMES(i, 0, 32, {CHECK_AND_RETURN((pinList[i] == pin), true);CHECK_AND_RUN_AND_RETURN((pinList[i] == 0), {pinList[i] = pin;}, false); });
  return true;
}

/*
* Function Name: 	init(int clkPin, int sdoPin)
* Purpose: 			  HX711驱动初始化（80HZ采样率）
* Params :        (int)clkPin HX711对应的时钟信号
*                 (int)sdoPin HX711对应的数据信号
* Return: 			  无
*/
void HX711::init(int clkPin, int sdoPin)
{
  this->clkPin = clkPin;
  this->sdoPin = sdoPin;
  CHECK_AND_RUN((!HX711::ckGpioIsInited(sdoPin)), GPIO_SET_MODE(sdoPin, 0));
  CHECK_AND_RUN((!HX711::ckGpioIsInited(clkPin)), {GPIO_SET_MODE(clkPin, 1); GPIO_SET_VAL(clkPin, 0); });
}

/*
* Function Name: 	getVal(bool isShowMsg)
* Purpose: 			  阻塞并读取压力值，最大阻塞时间20ms
* Params :        (bool)isShowMsg 是否打印调试信息
* Return: 			  (int)读取到的压力值
* Attention:      要达到80HZ的采样速率，要保证HX711的频率配置引脚已给对应电平
*/
int HX711::getVal(bool isShowMsg)
{
  static unsigned int lastTickMs = 0;  
  int count = 0;
  unsigned int ms = GET_TICK_MS();

  GPIO_SET_VAL(clkPin, 0);

  while (GPIO_GET_VAL(sdoPin) == 1 && (GET_TICK_MS() - ms <= 20))  //采样率80Hz（12ms周期），这里最大给20ms延时
    MARLIN_CORE_IDLE();

  DIASBLE_ALL_ISR();
  for (int i = 0; i < 24; i++)
  {
    GPIO_SET_VAL(clkPin, 1);
    count = count << 1;
    GPIO_SET_VAL(clkPin, 0);
    CHECK_AND_RUN((GPIO_GET_VAL(sdoPin) == 1), (count++));
  }
  
  GPIO_SET_VAL(clkPin, 1);
	count |= ((count & 0x00800000) != 0 ? 0xFF000000 : 0); //24位有符号，转成32位有符号
  GPIO_SET_VAL(clkPin, 0);
  ENABLE_ALL_ISR();

  CHECK_AND_RUN(isShowMsg, {PRINTF("T=%08d, S=%08d\n", (int)(GET_TICK_MS() - lastTickMs), (int)count);lastTickMs = GET_TICK_MS(); });

  return count;
}

/*
* Function Name: 	hFilter(double *vals, int count, double cutFrqHz, double acqFrqHz)
* Purpose: 			  对数据进行高通滤波
* Params :        (double*)vals    待滤波的数据
                  (int)count       待滤波的数据长度
                  (double)cutFrqHz 滤波器的截止频率
*                 (double)acqFrqHz 滤波器的采样频率（等同于HX711的采样频率80HZ）
* Return: 			  无
*/
void Filters::hFilter(double *vals, int count, double cutFrqHz, double acqFrqHz)
{
  double rc = 1.0f / 2.0f / PI / cutFrqHz;
  double coff = rc / (rc + 1 / acqFrqHz);
  double vi = vals[0], viPrev = vals[0], vo = 0, voPrev = 0;
  FOR_LOOP_TIMES(i, 0, count, {
    vi = vals[i];
    vo = (vi - viPrev + voPrev) * coff;
    voPrev = vo; 
    viPrev = vi;
    vals[i] = fabs(vo);
  });
}
/**冒泡排序
 *升序
 */
static void BubbleSort(double arr[],int len)
{
 int i,j;
 double tem;
 for(i=len-1;i>0;i--)
 {
     for(j=0;j<i;j++)
     {
        if(arr[j]>arr[j+1])
        {
            tem = arr[j];
            arr[j] = arr[j+1];
            arr[j+1] = tem;
        }
     }
 } 
}

/*
* Function Name: 	tFilter(double *vals, int count)
* Purpose: 			  对数据进行毛刺滤波
* Params :        (double)cutFrqHz 滤波器的截止频率
*                 (double)acqFrqHz 滤波器的采样频率（等同于HX711的采样频率80HZ）
* Return: 			  无
*/
void Filters::tFilter(double *vals, int count)
{
  // BubbleSort(vals,count);
  FOR_LOOP_TIMES(i, 0, count - 3, {
   double  minVal = (fabs(vals[i]) < fabs(vals[i+1]) ? vals[i] : vals[i+1]);
    vals[i] = fabs(minVal) < fabs(vals[i+2]) ? minVal : vals[i+2];});
}

/*
* Function Name: 	lFilter(double *vals, int count, double k1New)
* Purpose: 			  对数据进行低通滤波
* Params :        (double*)vals    待滤波的数据
                  (int)count       待滤波的数据长度
                  (double)k1New    一阶滤波参数
* Return: 			  无
*/
void Filters::lFilter(double *vals, int count, double k1New)
{
  FOR_LOOP_TIMES(i, 1, count, vals[i] = vals[i - 1] * (1 - k1New) + vals[i] * k1New);
}

/*
* Function Name: 	readBase()
* Purpose: 			  获取给定数量内(BASE_COUNT/2)的最大、最小、平均压力值。
* Params :        无
* Return: 			  (xyz_long_t) x=MIN; y=AVG; z=MAX;
*/
xyz_long_t ProbeAcq::readBase()
{
  static double vals[PI_COUNT / 2] = {0};

  double minVal = +0x00FFFFFF, avgVal = 0, maxVal = -0x00FFFFFF; // min avg max
  FOR_LOOP_TIMES(i, 0, PI_COUNT / 2, {this->hx711.getVal(false); });
  FOR_LOOP_TIMES(i, 0, PI_COUNT / 2, {vals[i] = this->hx711.getVal(false);});

  Filters::tFilter(vals, PI_COUNT / 2);
  Filters::lFilter(vals, PI_COUNT / 2, LFILTER_K1_NEW);

  ARY_MIN(minVal, vals, PI_COUNT / 2);
  ARY_MAX(maxVal, vals, PI_COUNT / 2);
  ARY_AVG(avgVal, vals, PI_COUNT / 2);
#if ENABLED(SHOW_MSG) 
  PRINTF("\n***BASE:MIN=%d, AVG=%d, MAX=%d***\n\n", (int)minVal, (int)avgVal, (int)maxVal);
#endif
  xyz_long_t xyz = {(int)minVal, (int)avgVal, (int)maxVal};
  return xyz;
}

/*
* Function Name: 	checHx711()
* Purpose: 			  检测HX711模块是否工作正常
* Params :        无
* Return: 			  true=正常；false=不正常；
* Attention:      理论上，HX711在一定时间内，最大与最值压力差应大于100，且小于MIN_HOLD。
*/
bool ProbeAcq::checHx711()
{
  xyz_long_t bv = readBase();
  return (abs(bv.x - bv.z) < 100 || abs(bv.x - bv.z) > MIN_HOLD) ? false : true;
}
// probeTimes(0, basePos_mm, 0.02, -10, 0, MIN_HOLD, MAX_HOLD));
float ProbeAcq::probeTimes(int max_times, xyz_float_t rdy_pos, float step_mm, float min_dis_mm, float max_z_err, int min_hold, int max_hold)
{
    ProbeAcq pa;
    float mm0 = 0, mm1 = 0;
    pa.hx711.init(HX711_SCK_PIN, HX711_SDO_PIN);
    pa.minZ_mm = min_dis_mm;
    pa.basePos_mm = rdy_pos;
    pa.baseSpdXY_mm_s = 100;        
    pa.baseSpdZ_mm_s = 5;
    pa.step_mm = step_mm;             
    pa.minHold = min_hold;
    pa.maxHold = max_hold;
    FOR_LOOP_TIMES(i, 0, (max_times <= 0 ? 1 : max_times), {
      mm0 = pa.probePointByStep()->outVal_mm;
      CHECK_AND_RETURN(max_times <= 0, mm0);
      mm1 = pa.probePointByStep()->outVal_mm;
      CHECK_AND_RETURN(fabs(mm0 - mm1) <= max_z_err, (mm0 + mm1) / 2);
    });
    return (mm0 + mm1) / 2;
}

/*
* Function Name: 	shakeZAxis(int times)
* Purpose: 			  振动Z轴以消除间隙应力
* Params :        (int)times 振动的次数
*/
void ProbeAcq::shakeZAxis(int times)
{
  float steps[] = STEPS_PRE_UNIT;
  int runStep = 0.01* steps[2];       //获取每步进step_mm距离，对应电机的脉冲数量
  runStep = runStep <= 0 ? 1 : runStep;

  int delayUs = (11.0f * 1000 / runStep) / 2;   //保证12ms左右，Z轴电机运动step_mm的距离
  delayUs = delayUs <= 10 ? 10 : delayUs;
  int dir = AXIS_Z_DIR_GET();
  FOR_LOOP_TIMES(i, 0, times, {
    AXIS_Z_DIR_SET(Z_DIR_ADD);        
    FOR_LOOP_TIMES(j, 0, 4 * runStep, AXIS_Z_STEP_PLUS(delayUs / 2));
    AXIS_Z_DIR_SET(Z_DIR_DIV); 
    FOR_LOOP_TIMES(j, 0, 4 * runStep, AXIS_Z_STEP_PLUS(delayUs / 2));
    MARLIN_CORE_IDLE();
  });
  AXIS_Z_DIR_SET(dir);
}

/*
* Function Name: 	calMinZ()
* Purpose: 			  根据触发后压力队列中的压力值，计算出对应的z轴高度
* Params :        无
* Return: 			  无
*/
void ProbeAcq::calMinZ()
{
  double* valP_t = &this->valP[PI_COUNT]; //rock_开始没有*2，数组越界使用 20230204
  double* posZ_t = &this->posZ[PI_COUNT]; //
  
  // 1、滤波  rock
  Filters::tFilter(this->valP, PI_COUNT * 2); 
  Filters::hFilter(this->valP, PI_COUNT * 2, RC_CUTE_FRQ, 80);
  Filters::lFilter(this->valP, PI_COUNT * 2, LFILTER_K1_NEW);

  // 2、打印结果
  #if ENABLED(SHOW_MSG)
  PRINTF("%s", "\nx=[");
  FOR_LOOP_TIMES(i, 0, PI_COUNT, PRINTF((i == (PI_COUNT - 1) ? "%s]\n\n" : "%s,"), getStr(posZ_t[i])));
  PRINTF("%s", "y=[");
  FOR_LOOP_TIMES(i, 0, PI_COUNT, PRINTF((i == (PI_COUNT - 1) ? "%s]\n\n" : "%s,"), getStr(valP_t[i])));
  #endif
  // 3、对数据进行归一化，方便处理
  double valMin = +0x00FFFFFF, valMax = -0x00FFFFFF;  
  ARY_MIN(valMin, valP_t, PI_COUNT);
  ARY_MAX(valMax, valP_t, PI_COUNT);
  FOR_LOOP_TIMES(i, 0, PI_COUNT, {valP_t[i] = (valP_t[i] - valMin) / (valMax - valMin);});

  // 4、计算并按给定角度翻转数据，以方便计算出最早的触发点
  double angle = atan((valP_t[PI_COUNT - 1] - valP_t[0]) / PI_COUNT);
  double sinAngle = sin(-angle), cosAngle = cos(-angle);
  FOR_LOOP_TIMES(i, 0, PI_COUNT, valP_t[i] = (i - 0) * sinAngle + (valP_t[i] - 0) * cosAngle + 0); //延原点(0,0)顺时针旋转angle度，这里可以不考虑X轴坐标值

  // 5、找出翻转后的最小值索引
  valMin = +0x00FFFFFF;
  ARY_MIN_INDEX(valMin, this->outIndex, valP_t, PI_COUNT);
  this->outVal_mm = posZ[this->outIndex + PI_COUNT];
#if ENABLED(SHOW_MSG)
  PRINTF("***CalZ Idx=%d, Z=%s***\n", this->outIndex, getStr(this->outVal_mm));
  #endif
}

/*
* Function Name: 	checkTrigger(int fitVal, int unfitDifVal)
* Purpose: 			  触发状态检测，用于检测喷头是否与压力传感器正常接触
* Params :        (int)fitVal RC低通滤波后的压力值，用于特征检测
*                 (int)unfitDifVal 未滤波但去零后的压力值，用于超压检测
* Return: 			  (bool) true=检测到解发或中止；false=未满足触发条件。
*/
bool ProbeAcq::checkTrigger()
{
  static double vp_t[PI_COUNT * 2] = {0};
  static double *valP_t = &vp_t[PI_COUNT]; //rock_20230204
  
  #if ENABLED(SHOW_MSG)
  //static long long lastTickMs = 0, lastUnfitDifVal = 0;  
  //PRINTF("T=%08d , S=%08d , U=%08d \n", (int)(GET_TICK_MS() - lastTickMs), fitVal, unfitDifVal);
  //lastTickMs = GET_TICK_MS();
  #endif

  // 0、复制一份用于检测，防止对源数据造成影响
  FOR_LOOP_TIMES(i, 0, PI_COUNT * 2, vp_t[i] = this->valP[i]);

  // 1、数据滤波  rock
  Filters::tFilter(vp_t, PI_COUNT * 2); 
  Filters::hFilter(vp_t, PI_COUNT * 2, RC_CUTE_FRQ, 80);
  Filters::lFilter(vp_t, PI_COUNT * 2, LFILTER_K1_NEW);

  // 2、最高优先级，到达最小位置还未触发，则强制停止
  CHECK_AND_RETURN((this->posZ[PI_COUNT - 1] <= this->minZ_mm), true);

  // 3、最高优先级，压力大于最大压力，则无条件停止。
  CHECK_AND_RETURN((abs(valP_t[PI_COUNT - 1]) > this->maxHold && abs(valP_t[PI_COUNT - 2]) > this->maxHold && abs(valP_t[PI_COUNT - 3]) > this->maxHold), true);

  // 4、检测点数未满足最小需求
  CHECK_AND_RETURN(this->valP[0] == 0, false);

  // 5、检测未尾3个点大小的是否有序递增
  CHECK_AND_RETURN((!((valP_t[PI_COUNT - 1] > valP_t[PI_COUNT - 2]) && (valP_t[PI_COUNT - 2] > valP_t[PI_COUNT - 3]))), false);

  // 6、检测未3个点绝对值是否大于其它任意点
  FOR_LOOP_TIMES(i, 0, PI_COUNT - 3, CHECK_AND_RETURN((valP_t[PI_COUNT - 1] < valP_t[i] || valP_t[PI_COUNT - 2] < valP_t[i] || valP_t[PI_COUNT - 3] < valP_t[i]), false));

  // 6、对数据进行归一化，方便处理
  double valMin = +0x00FFFFFF, valMax = -0x00FFFFFF, valAvg = 0;
  float valP_t_f[PI_COUNT] = {0};
  ARY_MIN(valMin, valP_t, PI_COUNT);
  ARY_MAX(valMax, valP_t, PI_COUNT);
  ARY_AVG(valAvg, valP_t, PI_COUNT);
  FOR_LOOP_TIMES(i, 0, PI_COUNT, valP_t_f[i] =((double)valP_t[i] - valMin) / (valMax - valMin));

  // 7、保证所有点针对最后一个点的斜率均大于40度,可有郊防止过于灵敏而导致的误触发。
  FOR_LOOP_TIMES(i, 0, PI_COUNT - 1, CHECK_AND_RETURN(((valP_t_f[PI_COUNT - 1] - valP_t_f[i]) / ((32 - i) * 1.0f / 32) < 0.8), false));

  // 8、限制最小值，防止误触发。
  CHECK_AND_RETURN(!(valP_t[PI_COUNT - 1] > this->minHold && valP_t[PI_COUNT - 2] > (this->minHold / 2) && valP_t[PI_COUNT - 3] > (this->minHold / 3)), false);
  return true;
}

/*
* Function Name: 	probePointByStep()
* Purpose: 			  单步进法测量，并返回测量结果。
* Params :        无
* Return: 			  (ProbeAcq) this指针
*/
ProbeAcq* ProbeAcq::probePointByStep()
{
  static char msg[128] = {0};
  int unfitAvgVal = 0, nowVal = 0,  runSteped = 0;

  this->outIndex = PI_COUNT - 1;
  this->outVal_mm = 0;

  #if ENABLED(SHOW_MSG)
    PRINTF("\nPROBE: rdyX=%s, rdyY=%s, rdyZ=%s, spdXY_mm_s=%s, spdZ_mm_s=%s",
                          getStr(this->basePos_mm.x), getStr(this->basePos_mm.y), getStr(this->basePos_mm.z), getStr(this->baseSpdXY_mm_s), getStr(this->baseSpdZ_mm_s));
    PRINTF("len_mm=%s, baseCount=%d, minHold=%d, maxHold=%d, step_mm=%s\n\n",
                          getStr(this->minZ_mm), PI_COUNT, this->minHold, this->maxHold, getStr(this->step_mm)); 
  #endif

  memset(this->posZ, 0, sizeof(this->posZ));
  memset(this->valP, 0, sizeof(this->valP));

  memset(msg, 0, sizeof(msg));
  sprintf(msg, "G1 F%s X%s Y%s Z%s", getStr(this->baseSpdXY_mm_s * 60), getStr(this->basePos_mm.x), getStr(this->basePos_mm.y), getStr(this->basePos_mm.z));
  RUN_AND_WAIT_GCODE_CMD(msg, true);                  //运动到等测量点（准备点）

  int oldBedTmp = GET_BED_TAR_TEMP();
  int oldHotTmp = GET_HOTEND_TAR_TEMP(0);
  SET_BED_TEMP(0);
  SET_HOTEND_TEMP(0, 0);

  unfitAvgVal = readBase().y;                   //获取干净的数据

  float steps[] = STEPS_PRE_UNIT;               // { 80, 80, 400, 424.9 }
  int runStep = this->step_mm * steps[2];       //获取每步进step_mm距离，对应电机的脉冲数量
  runStep = runStep <= 0 ? 1 : runStep;
  int delayUs = (12.0f * 1000 / runStep) / 2;   //保证12ms左右，Z轴电机运动step_mm的距离
  delayUs = delayUs <= 10 ? 10 : delayUs;

  this->shakeZAxis(20);

  AXIS_Z_ENABLE();
  int dir = AXIS_Z_DIR_GET();
  AXIS_Z_DIR_SET(Z_DIR_DIV);
  do{
    FOR_LOOP_TIMES(i, 0, runStep, AXIS_Z_STEP_PLUS(delayUs)); //向给定方向运动给定步进距离
    runSteped += runStep;
    MARLIN_CORE_IDLE();
    nowVal = this->hx711.getVal(0);
    FOR_LOOP_TIMES(i, 0, PI_COUNT * 2 - 1, this->valP[i] = this->valP[i + 1]);  
    FOR_LOOP_TIMES(i, 0, PI_COUNT * 2 - 1, this->posZ[i] = this->posZ[i + 1]);
    this->valP[PI_COUNT * 2 - 1] = nowVal - unfitAvgVal;                 //将压力值放入队列
    this->posZ[PI_COUNT * 2 - 1] = current_position[2] - (float)runSteped / steps[2];   //将压力值对应的z位置放入队列

    if (checkTrigger() == true) //触发条件检测
    {
      calMinZ();          //数据整理与计算
      AXIS_Z_DIR_SET(Z_DIR_ADD);
      FOR_LOOP_TIMES(i, 0, runSteped, AXIS_Z_STEP_PLUS(delayUs / 4)); //返回到触发点或准备点
      AXIS_Z_DIR_SET(dir);
      break;
    }
  } while (1);
  SET_BED_TEMP(oldBedTmp);
  SET_HOTEND_TEMP(oldHotTmp, 0);
  return this;
}

/*
* Function Name: 	clearByBrush(xyz_float_t basePos_mm, float norm, float minTemp, float maxTemp)
* Purpose: 			  触发状态检测，用于检测喷头是否与压力传感器正常接触
* Params :        (xyz_float_t)basePos_mm 喷头擦式过程中，毛刷的坐标
*                 (float)norm 喷头擦式过程中，喷头前后左右移动的距离，与毛刷的大小有关
*                 (float)minTemp 在此温度及以下，才可保证耗材不再渗出
*                 (float)maxTemp 在此温度及以上，才可保证耗材已融化
* Return: 			  (bool) true=擦式成功；false=擦式失败。
* Attention:      由于喷头擦式过程中，需要融化耗材，所在此处的minTemp和maxTemp需与GCODE中的温度相对应，或与上一次使用的耗材类型对应
*/
bool clearByBed(xyz_float_t startPos, xyz_float_t endPos, float minTemp, float maxTemp)
{
  ProbeAcq pa;
  pa.hx711.init(HX711_SCK_PIN, HX711_SDO_PIN);
  DO_BLOCKING_MOVE_TO_Z(startPos.z, 5);
  DO_BLOCKING_MOVE_TO_XY(startPos.x, startPos.y-10, 50);
  Popup_Window_Height(Nozz_Hot);  //刷新对高页面显示_喷嘴加热中
  SET_HOTEND_TEMP(maxTemp, 0);
  SET_BED_TEMP(65);  //暂时取消热床加热
 
  WAIT_HOTEND_TEMP(60 * 5 * 1000, 5);//等待温度达到设定值
  WAIT_BED_TEMP(60 * 5 * 1000, 2);
  Popup_Window_Height(Nozz_Clear);  //刷新对高页面显示_擦喷嘴中
  In_out_feedtock_level(LEVEL_DISTANCE,FEEDING_DEF_SPEED,false); //回抽50mm
  DO_BLOCKING_MOVE_TO_XY(startPos.x, startPos.y, 50);
  float start_mm = ProbeAcq::probeTimes(3, startPos, 0.03, -10, 0.2, MIN_HOLD, MAX_HOLD/2);
  PRINT_LOG("clear_nozzle_start_z:",start_mm);
  float end_mm=    ProbeAcq::probeTimes(3, endPos, 0.03, -10, 0.2, MIN_HOLD, MAX_HOLD/2);
  PRINT_LOG("clear_nozzle_end_z:",end_mm);
  startPos.z=start_mm;endPos.z=end_mm;
  // PRINT_LOG("clear_nozzle_start_z:",start_mm,"clear_nozzle_start_z:",end_mm);
  DO_BLOCKING_MOVE_TO_XYZ(startPos.x, startPos.y+3, startPos.z, 50);
  // SET_HOTEND_TEMP(maxTemp, 0);
  // WAIT_HOTEND_TEMP(60 * 5 * 1000, 5);//等待温度达到设定值
  // RUN_AND_WAIT_GCODE_STR("G1 F500 X%s Y%s z%s", true, getStr(startPos.x), getStr(startPos.y),getStr(startPos.z));
  // DO_BLOCKING_MOVE_TO_XYZ(endPos.x, endPos.y, endPos.z-0.1, 5); //往前移动3mm，防止上次有耗材。再次粘连
  DO_BLOCKING_MOVE_TO_XYZ(endPos.x, endPos.y-3, endPos.z-0.1, 5);
  endPos.x-=10;endPos.y-=10;

  DO_BLOCKING_MOVE_TO_XYZ(endPos.x, endPos.y, endPos.z-0.1, 5);//往回45°拉一点，撤掉余料
  RUN_AND_WAIT_GCODE_CMD("G28 Z", true);
  /*
  int unfitAvgVal = pa.readBase().y; 

  // RUN_AND_WAIT_GCODE_STR("G1 F100 X%s Y%s z%s", false, getStr(startPos.x), getStr(startPos.y),getStr(start_mm));
  DO_BLOCKING_MOVE_TO_XY(startPos.x, startPos.y, 50);
  DO_BLOCKING_MOVE_TO_Z(start_mm+0.2 , 5);
  SET_HOTEND_TEMP(minTemp, 0);
  
  // RUN_AND_WAIT_GCODE_STR("G1 F100 X%s Y%s", false, getStr(endPos.x), getStr(endPos.y));
  RUN_AND_WAIT_GCODE_STR("G1 F200 X%s Y%s z%s", false, getStr(endPos.x), getStr(endPos.y),getStr(end_mm));
  
  SET_HOTEND_TEMP(minTemp, 0);
  SET_FAN_SPD(255);

  float steps[] = STEPS_PRE_UNIT;
  int runStep = 0.02 * steps[2];
  int delayUs = (12.0f * 1000 / runStep) / 2;

  int oldDir = AXIS_Z_DIR_GET();
  AXIS_Z_ENABLE();

  while (AXIS_XYZE_STATUS()){
    int nowVal = pa.hx711.getVal(0);
    // PRINT_LOG("nowVal:",nowVal,"unfitAvgVal:",unfitAvgVal,"nowVal - unfitAvgVal:",abs(nowVal - unfitAvgVal));
    CHECK_AND_RUN_AND_ELSE((abs(nowVal - unfitAvgVal) < (MIN_HOLD)), AXIS_Z_DIR_SET(Z_DIR_DIV), AXIS_Z_DIR_SET(Z_DIR_ADD));  
    FOR_LOOP_TIMES(i, 0, runStep, AXIS_Z_STEP_PLUS(delayUs));
    MARLIN_CORE_IDLE();
  }
  AXIS_Z_DIR_SET(oldDir);
  SET_HOTEND_TEMP(minTemp, 0);
  SET_FAN_SPD(255);                             //打开模型散热风扇，保证更快速度的降温
  WAIT_HOTEND_TEMP(60 * 5 * 1000, 5);           //等待温度达到设定值
  AXIS_Z_DIR_SET(Z_DIR_ADD);
  // PRINT_LOG("endPos.x = ", endPos.x + (endPos.x - startPos.x) / 5," endPos.y = ", endPos.y + (endPos.y - startPos.y) / 5," GET_CURRENT_POS().z + 3.5 = ",GET_CURRENT_POS().z + 3.5); 
  RUN_AND_WAIT_GCODE_STR("G1 F300 X%s Y%s Z%s", true, getStr(endPos.x + (endPos.x - startPos.x) / 5), getStr(endPos.y + (endPos.y - startPos.y) / 5), getStr(GET_CURRENT_POS().z + 3));

  
  pa.shakeZAxis(100);
  AXIS_Z_DIR_SET(Z_DIR_ADD);
  RUN_AND_WAIT_GCODE_CMD("G28 Z", true);

  SET_HOTEND_TEMP(minTemp, 0);
  // SET_BED_TEMP(60);
  */
  return true;
}

/*
* Function Name: 	probeByPress(xyz_float_t basePos_mm, float* outZ)
* Purpose: 			  使用压力传感器对喷头高度测量
* Params :        (xyz_float_t)basePos_mm 测量过程中，喷头位于压力传感器正中的坐标
*                 (float*)outZ 测量结果，共测量两次，取平均值。
* Return: 			  (bool) true=测量成功；false=测量失败。
* Attention:      若喷头未擦式干净并粘有耗材，会导致测量失败。
*/
bool probeByPress(xyz_float_t basePos_mm, float* outZ)
{
  /*
  float outZ_mm[5] = {0};
  //CHECK_AND_RETURN((!(pa.checHx711() || pa.checHx711())), false); //检测模块工作是否正常
  FOR_LOOP_TIMES(i, 0, 5, outZ_mm[i] = ProbeAcq::probeTimes(0, basePos_mm, 0.02, -10, 0, MIN_HOLD, MAX_HOLD));
  ARY_SORT(outZ_mm, 5);
  PRINTF("\n***PROBE BY PRESS: z=%s, zs={%s, %s, %s, %s, %s}***\n", getStr(outZ_mm[2]), getStr(outZ_mm[0]), getStr(outZ_mm[1]), getStr(outZ_mm[2]), getStr(outZ_mm[3]), getStr(outZ_mm[4]));
  *outZ = outZ_mm[2];
  */
 //为了节约时间，由原来的5次改成3次
   float outZ_mm[3] = {0};
  //CHECK_AND_RETURN((!(pa.checHx711() || pa.checHx711())), false); //检测模块工作是否正常
  FOR_LOOP_TIMES(i, 0, 3, outZ_mm[i] = ProbeAcq::probeTimes(0, basePos_mm, 0.02, -10, 0, MIN_HOLD, MAX_HOLD));
  ARY_SORT(outZ_mm, 3);
  #if ENABLED(SHOW_MSG)
  PRINTF("\n***PROBE BY PRESS: z=%s, zs={%s, %s, %s}***\n", getStr(outZ_mm[1]), getStr(outZ_mm[0]), getStr(outZ_mm[1]), getStr(outZ_mm[2]));
  #endif
  *outZ = outZ_mm[1];
  PRINTF("\n***PROBE BY PRESS: press_z=%s***\n", getStr(*outZ));
  return true;
}

/*
* Function Name: 	probeByTouch(xyz_float_t rdyPos_mm, float* outZ)
* Purpose: 			  使用CR-TOUCH测量触发高度
* Params :        (xyz_float_t)basePos_mm 测量过程中，CR-TOUCH位于压力传感器正中的坐标
*                 (float*)outZ 测量结果，共测量两次，取第二次。
* Return: 			  (bool) true=测量成功；false=测量失败。
* Attention:      CR-TOUCH的测量流程，要与HOME点的测量流程相一致。
*/
bool probeByTouch(xyz_float_t rdyPos_mm, float* outZ)
{
  ProbeAcq pa;
  pa.shakeZAxis(20);  
  xyz_float_t touchOftPos = CRTOUCH_OFT_POS;
  int oldNozTmp = GET_NOZZLE_TAR_TEMP(0);
  int oldBedTmp = GET_BED_TAR_TEMP();

  DO_BLOCKING_MOVE_TO_Z(rdyPos_mm.z, 5);
  DO_BLOCKING_MOVE_TO_XY(rdyPos_mm.x - touchOftPos.x, rdyPos_mm.y - touchOftPos.y, 100);
  *outZ = PROBE_PPINT_BY_TOUCH(rdyPos_mm.x - touchOftPos.x, rdyPos_mm.y - touchOftPos.y);   //调用Marlin固有的CR-TOUCH测量接口
  PRINTF("\n***PROBE BY TOUCH: touch_z=%s***\n", getStr(*outZ));
 
  SET_HOTEND_TEMP(oldNozTmp, 0);
  SET_BED_TEMP(oldBedTmp);

  probe.stow(); //rock
  return true;
}

/*
* Function Name: 	printTestResult(float *zTouch, float *zPress)
* Purpose: 			  打印测试结果
* Params :        (float*)zTouch TOUCH传感器的测量结
*                 (float*)zPress 压力传感器的测量结果
* Return: 			  无
* Attention:      最大只能打印128组测试结果
*/
void printTestResult(float *zTouch, float *zPress)
{
  static float acqVals[128][3] = {0};    //最大保存128组测量数据
  static int acqValIndex = 0;

  PRINTF("\n***GET Z OFFSET: zTouch={%s, %s, %s}, zPress={%s, %s, %s}, zOffset={%s, %s, %s}***\n", 
    getStr(zTouch[0]), getStr(zTouch[1]), getStr(zTouch[2]), getStr(zPress[0]), getStr(zPress[1]), getStr(zPress[2]), 
    getStr(zPress[0] - zTouch[0]), getStr(zPress[1] - zTouch[1]), getStr(zPress[2] - zTouch[2]));

  float zt_avg = 0, zp_avg = 0;
  ARY_AVG(zt_avg, zTouch, 3);
  ARY_AVG(zp_avg, zPress, 3);

  acqVals[acqValIndex][0] = zp_avg;
  acqVals[acqValIndex][1] = zt_avg;
  acqVals[acqValIndex][2] = zp_avg - zt_avg;
  acqValIndex = (acqValIndex >= 127 ? 127 : (acqValIndex + 1));
  FOR_LOOP_TIMES(i, 0, acqValIndex, PRINTF("%d\t%s\t%s\t%s\n", i, getStr(acqVals[i][0]), getStr(acqVals[i][1]), getStr(acqVals[i][2])));  
}
//一次对高
float Hight_One(xyz_float_t pressPos)
{
  float temp_value=0,zoffset_avg=0;
  float zTouch[1] = {0};
  float zPress[1] = {0};
  
  bool isRunProByPress=true, isRunProByTouch=true;
  SET_BED_LEVE_ENABLE(false);  
  // CHECK_AND_RUN(isRunProByTouch, {FOR_LOOP_TIMES(i, 0, 3, {probeByTouch(rdyPos[i], &zTouch[i]); rdyPos[i].z = zTouch[i];})});       
  CHECK_AND_RUN(isRunProByTouch, {FOR_LOOP_TIMES(i, 0, 1, {probeByTouch(pressPos, &zTouch[0]); pressPos.z = zTouch[0];})});
  //3. 使用压力传感器对喷头高度进行测量
      
  SET_BED_LEVE_ENABLE(false);                                     
  // CHECK_AND_RUN(isRunProByPress, FOR_LOOP_TIMES(i, 0, 3, {probeByPress(rdyPos[i], &zPress[i]); zPress[i] += NOZ_TEMP_OFT_MM;})); 
  CHECK_AND_RUN(isRunProByPress, FOR_LOOP_TIMES(i, 0, 1, {probeByPress(pressPos, &zPress[0]); zPress[0] += NOZ_TEMP_OFT_MM;}));      
  //4. 处理结果    
  temp_value = (zPress[0] - zTouch[0]);
  printTestResult(zTouch, zPress);      
  DO_BLOCKING_MOVE_TO_Z(5, 5);    
  return temp_value;
}

//多次对高函数 Multiple  暂时先实现两次对高
float Multiple_Hight(bool isRunProByPress, bool isRunProByTouch)
{
  float zoffset_value[3]={0};
  uint8_t loop_max=0,loop_num=0;
  xyz_float_t pressPos = PRESS_XYZ_POS;
  float temp_value=0,temp_zoffset=0,temp_zoffset1=0,zoffset_avg=0;
   for(loop_num=0;loop_num<ZOFFSET_REPEAT_NIN;loop_num++)
   {  
    pressPos.y-=(loop_num*5);    
     zoffset_value[loop_num]=Hight_One(pressPos);
     PRINTF("\n***OUTPUT_ZOFFSET: zOffset=%s***\n", getStr(zoffset_value[loop_num]));
     RUN_AND_WAIT_GCODE_CMD("G28", true);                        //测量前先获取一次HOME点 
   }
   temp_zoffset=fabs(zoffset_value[0])-fabs(zoffset_value[1]);
   if(fabs(temp_zoffset)<=ZOFFSET_COMPARE) //采集到的值统一
   {
     ARY_AVG(zoffset_avg, zoffset_value, 2);
     SET_BED_LEVE_ENABLE(true);  //打开自动调平
     In_out_feedtock_level(LEVEL_DISTANCE,FEEDING_DEF_SPEED,true); //进料50mm
     SET_HOTEND_TEMP(140, 0);
     SET_FAN_SPD(255);                             //打开模型散热风扇，保证更快速度的降温
     WAIT_HOTEND_TEMP(60 * 5 * 1000, 5);           //等待温度达到设定值
     Popup_Window_Height(Nozz_Finish); //对高完成刷新页面 
     return zoffset_avg; 
   }  
   else //如果不统一就多调试几次
   {
      for(loop_max=2; loop_max<ZOFFSET_REPEAT_NAX;loop_max++)
      {
        pressPos.y-=(loop_max-2)*5;
        zoffset_value[2]=Hight_One(pressPos); 
        temp_zoffset=fabs(zoffset_value[2])-fabs(zoffset_value[0]);
        temp_zoffset1=fabs(zoffset_value[2])-fabs(zoffset_value[1]);
        if((fabs(temp_zoffset)>ZOFFSET_COMPARE)&&(fabs(temp_zoffset1)>ZOFFSET_COMPARE))//两个都不在范围内
        {
          continue;  //返回继续采集
        }
        else if((fabs(temp_zoffset)<=ZOFFSET_COMPARE)||(fabs(temp_zoffset1)<=ZOFFSET_COMPARE))
        {
          if((fabs(temp_zoffset)<=ZOFFSET_COMPARE))
          {
            zoffset_value[1]=zoffset_value[2];
          }
          else 
          {
            zoffset_value[0]=zoffset_value[2];
          }
          ARY_AVG(zoffset_avg, zoffset_value, 2);
          SET_BED_LEVE_ENABLE(true);  //打开自动调平
          In_out_feedtock_level(LEVEL_DISTANCE,FEEDING_DEF_SPEED,true); //进料50mm
          SET_HOTEND_TEMP(140, 0);
          SET_FAN_SPD(255);                             //打开模型散热风扇，保证更快速度的降温
          WAIT_HOTEND_TEMP(60 * 5 * 1000, 5);           //等待温度达到设定值
          Popup_Window_Height(Nozz_Finish);             //对高完成刷新页面 
          return zoffset_avg; 
        }        
      }
      //防止Z轴补偿有-0.04的情况
      if(ZOFFSET_REPEAT_NAX==loop_max) //防止输出是0，最后直接算一个。
      {
          /*  有弹窗提示客户调平失败，就取消这段，一直得到最后的值
          ARY_AVG(zoffset_avg,zoffset_value,2);
          SET_BED_LEVE_ENABLE(true);                                    //打开自动调平
          In_out_feedtock_level(LEVEL_DISTANCE,FEEDING_DEF_SPEED,true); //进料50mm
          SET_HOTEND_TEMP(140, 0);
          SET_FAN_SPD(255);                             //打开模型散热风扇，保证更快速度的降温
          WAIT_HOTEND_TEMP(60 * 5 * 1000, 5);           //等待温度达到设定值
          Popup_Window_Height(Nozz_Finish);             //对高完成刷新页面 
          */
          return zoffset_avg; 
      }
      return zoffset_avg; //对高失败 
   }   
  //  CHECK_AND_RUN((isRunProByPress && isRunProByTouch), SET_Z_OFFSET((zPress[0] - zTouch[0]), false));  
}
/*
* Function Name: 	getZOffset(float* outOffset)
* Purpose: 			  包括喷头擦式、压力传感器测量喷头高度、CR-TOUCH高度测量三个部分，并返回Z-OFFSET
* Params :        (float*)outOffset 测量结果
* Return: 			  (bool) true=测量成功，outZ可用于设定系统补偿；false=测量失败。
* Attention:      测量失败，则不要设置系统Z-OFFSET。
*/
bool getZOffset(bool isNozzleClr, bool isRunProByPress, bool isRunProByTouch, float* outOffset)
{
  xyz_float_t pressPos = PRESS_XYZ_POS;
  xyz_float_t touchOftPos = CRTOUCH_OFT_POS;
  xyz_float_t rdyPos[3] = {{0, 0, HIGHT_UPRAISE_Z}, {0, 0, HIGHT_UPRAISE_Z}, {0, 0, HIGHT_UPRAISE_Z}};
  rdyPos[0].x = rdyPos[2].x = (pressPos.x < (BED_SIZE_X_MM / 2) ? (touchOftPos.x < 0) : (touchOftPos.x > 0)) ? pressPos.x : touchOftPos.x + 10;
  // rdyPos[0].x = ((pressPos.x < (BED_SIZE_X_MM / 2) ? (touchOftPos.x < 0) : (touchOftPos.x > 0)) ? pressPos.x : touchOftPos.x + 10);//-30
  rdyPos[0].x -= 9; 
  rdyPos[0].y = rdyPos[1].y = ((pressPos.y < (BED_SIZE_Y_MM / 2) ? (touchOftPos.y < 0) : (touchOftPos.y > 0)) ? pressPos.y : touchOftPos.y + 10);//-15
  rdyPos[0].y = rdyPos[1].y-=17;
  rdyPos[1].x = rdyPos[0].x + (pressPos.x < (BED_SIZE_X_MM / 2) ? +1 : -1) * BED_SIZE_X_MM / 5;
  rdyPos[2].y = rdyPos[0].y + (pressPos.y < (BED_SIZE_Y_MM / 2) ? +1 : -1) * BED_SIZE_Y_MM / 5;
  rdyPos[2].x = (rdyPos[0].x+(rdyPos[1].x-rdyPos[0].x)/2); 
//  PRINT_LOG("(rdyPos[1].x-rdyPos[0].x)/2:",(rdyPos[1].x-rdyPos[0].x)/2);
//   PRINT_LOG("rdyPos[0].x:",rdyPos[0].x,"rdyPos[1].x:",rdyPos[1].x,"rdyPos[2].x:",rdyPos[2].x);
   bool SoftEndstopEnable = soft_endstop._enabled;
  bool reenable = planner.leveling_active;
  //0. 测量前的准备工作
  SET_Z_OFFSET(0, false);                               //先对z-offset清0，防止当z-offset对测量结果造成影响
  RUN_AND_WAIT_GCODE_CMD("G28", true);                        //测量前先获取一次HOME点  

  //1. 针对PLA耗材进行喷头擦式
  srand(millis());
  float ret1 = rand() % 15 + 2; //生成1~10的随机数
  // SERIAL_ECHOLNPAIR(" ret1=: ",ret1);
  xyz_float_t startPos = {rdyPos[0].x + (rdyPos[1].x - rdyPos[0].x) * 1 / 5-10, rdyPos[0].y + (rdyPos[2].y - rdyPos[0].y) * 2 / 5 + random(0, 9) - 4, 6};
  // xyz_float_t startPos = {rdyPos[0].x + (rdyPos[1].x - rdyPos[0].x) * 1 / 5-10, rdyPos[0].y + (rdyPos[2].y - rdyPos[0].y) * 2 / 5 + 9 - 4, 6};
  xyz_float_t endPos =   {rdyPos[0].x + (rdyPos[1].x - rdyPos[0].x) * 3 / 5, startPos.y, 6};
  // startPos.x+=(ret1+15);startPos.y+=(ret1+5);
  // endPos.x+=(ret1+15);endPos.y=startPos.y;
   startPos.x=CLEAR_NOZZL_START_X;startPos.y=CLEAR_NOZZL_START_Y;
   endPos.x=CLEAR_NOZZL_END_X;endPos.y=CLEAR_NOZZL_END_Y;
  startPos.z=0;
  endPos.z=0;
  
  CHECK_AND_RUN(isNozzleClr, clearByBed(startPos, endPos, 140, 200)); 
  Popup_Window_Height(Nozz_Hight);  //刷新对高页面显示_对高测量中

  //2. 使用CR-TOUCCH测量触发高度
/*
  float zTouch[3] = {0};
  SET_BED_LEVE_ENABLE(false);  
  // CHECK_AND_RUN(isRunProByTouch, {FOR_LOOP_TIMES(i, 0, 3, {probeByTouch(rdyPos[i], &zTouch[i]); rdyPos[i].z = zTouch[i];})});       
  CHECK_AND_RUN(isRunProByTouch, {FOR_LOOP_TIMES(i, 0, 1, {probeByTouch(pressPos, &zTouch[0]); pressPos.z = zTouch[0];})});
  //3. 使用压力传感器对喷头高度进行测量
  float zPress[3] = {0};
  SET_BED_LEVE_ENABLE(false);                                     
  // CHECK_AND_RUN(isRunProByPress, FOR_LOOP_TIMES(i, 0, 3, {probeByPress(rdyPos[i], &zPress[i]); zPress[i] += NOZ_TEMP_OFT_MM;})); 
  CHECK_AND_RUN(isRunProByPress, FOR_LOOP_TIMES(i, 0, 1, {probeByPress(pressPos, &zPress[0]); zPress[0] += NOZ_TEMP_OFT_MM;}));      
  Popup_Window_Height(Nozz_Finish); //对高完成刷新页面
  //4. 处理结果
  float zt_avg = 0, zp_avg = 0;
  //一次就不用平均
  // ARY_AVG(zt_avg, zTouch, 3);
  // ARY_AVG(zp_avg, zPress, 3);
 
  ARY_AVG(zt_avg, zTouch, 1);
  ARY_AVG(zp_avg, zPress, 1);
  printTestResult(zTouch, zPress);  
  SET_BED_LEVE_ENABLE(true);
  DO_BLOCKING_MOVE_TO_Z(5, 5);
  // DO_BLOCKING_MOVE_TO_XY(BED_SIZE_X_MM / 2, BED_SIZE_Y_MM / 2, 50);  // 取消无用的步骤 rock_20230528
  *outOffset = (zp_avg - zt_avg);
*/
  *outOffset=Multiple_Hight(isRunProByPress,isRunProByTouch);  //测量两次后得到数据
  PRINTF("\n***OUTPUT_ZOFFSET: zOffset=%s***\n", getStr(*outOffset));
  
  //LIMIT(*outOffset, ZOFFSET_VALUE_MIN, ZOFFSET_VALUE_MAX);
  
  soft_endstop._enabled = SoftEndstopEnable;
  planner.leveling_active = reenable;
  if((*outOffset>ZOFFSET_VALUE_MAX)||(*outOffset<ZOFFSET_VALUE_MIN))return false; //如果对高值太离谱直接不要 
  return (isRunProByPress && isRunProByTouch);  //喷头和CR-TOUCH都测量，数据才有效
}

// /*
// * Function Name: 	gcodeG212()
// * Purpose: 			  用于上位机发送测试指令，如G212 C128 B P T， C128=测量128次；B=执行擦头操作；P=执行压力优传感器测量操作；T=执行CR-TOUCH测量操作
// * Params :        无
// * Return: 			  无
// * Attention:      需要在gcode.h中添加 #include "../module/AutoOffset.h"， 在gcode.cpp的 case 'G':的break前插入case 212:gcodeG212();break;
// */
// void gcodeG212()
// {
//   int count = 1;
//   if(GET_PARSER_SEEN('C')) count = GET_PARSER_INT_VAL();

//   if(GET_PARSER_SEEN('H'))
//   {
//     HX711 hx711;
//     hx711.init(HX711_SCK_PIN, HX711_SDO_PIN);
//     FOR_LOOP_TIMES(i, 0, count, hx711.getVal(1)); 
//     return;
//   }

//   if(GET_PARSER_SEEN('K'))
//   {
//     ProbeAcq pa;
//     xyz_pos_t cp = PRESS_XYZ_POS;
//     pa.hx711.init(HX711_SCK_PIN, HX711_SDO_PIN);
//     pa.minZ_mm = -10;    //最多下降10mm
//     pa.basePos_mm.x = cp.x;
//     pa.basePos_mm.y = cp.y;
//     pa.basePos_mm.z = 3;
//     pa.baseSpdXY_mm_s = 100;        
//     pa.baseSpdZ_mm_s = 5;
//     pa.step_mm = 0.02;  //rock             
//     pa.minHold = MIN_HOLD;
//     pa.maxHold = MAX_HOLD;
//     pa.probePointByStep();
//     return;
//   }

//   bool isRunClearNozzle = GET_PARSER_SEEN('B'); //是否需要擦喷头
//   bool isRunTestByPress = GET_PARSER_SEEN('P'); //是否需要使用压力传感器测量喷头高度
//   bool isRunTestByTouch = GET_PARSER_SEEN('T'); //是否需要使用CT-TOUCH测量
//   float zOffset = 0;
//   CHECK_AND_RUN((isRunClearNozzle || isRunTestByPress || isRunTestByTouch), FOR_LOOP_TIMES(i, 0, count, getZOffset(isRunClearNozzle, isRunTestByPress, isRunTestByTouch, &zOffset)));

// }

//对高操作再次封装

#endif