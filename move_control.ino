#include<string.h>
#include <ros.h>
//！电机差速控制
class STEPPER
{
  public:
    byte DirPin, PulPin, EnablePin,dir_bug;
    long CurrentStepTime = 22360, TargetStepTime = 1000, LastStepSt = 0;//微秒
    long CurrentStep = 0, TargetStep = 0, StartStep = 0;
    long Acceleration = 1000;//加速度(步/秒~2）
    long SpeedUpStep=0, SpeedDownStep=0;//加速,减速时的步数
    bool IsDebug = false;//调试使能
    bool flag = 0;
    long Current_v = 0;
    
    //初始化引脚
    STEPPER(byte dir, byte pul, byte en):DirPin(dir),PulPin(pul),EnablePin(en)
    {
      calCulateSpeedChangeMinimunDis();//初始化目标速度和单程变速位移

      pinMode(DirPin, OUTPUT);
      pinMode(PulPin, OUTPUT);
      pinMode(EnablePin, OUTPUT);
      digitalWrite(DirPin, 0);
      digitalWrite(PulPin, 0);
      digitalWrite(EnablePin, 0);
    }

    void SetTargetStepTime(long targetsteptime)//设置速度
    {
      TargetStepTime = targetsteptime;
      calCulateSpeedChangeMinimunDis();
    }

    void SetAcceleration(long acceleration)//设置加速度
    {
      Acceleration = acceleration;
      calCulateSpeedChangeMinimunDis();
    }
    
    bool IsRunToTarget(){return TargetStep == CurrentStep;}
    bool CanRun(){return micros() - LastStepSt >= CurrentStepTime;}//注：micros记录微秒数
    //目标驱动操作
    bool RunTo(long target)
    {
      flag = 0;
      //对于新目标值，要重设步数
      if (TargetStep != target)
      {
        TargetStep = target;
        CurrentStepTime = 0;
        StartStep = CurrentStep;
        calCulateSpeedChangeMinimunDis();//更新目标速度和理论单程位移
        if (IsDebug)
        { /*
            Serial.print ("V: ");
            Serial.println (1000000.0 / TargetStepTime);

            Serial.print ("_speedChangeMinimumDis: ");
            Serial.println (_speedChangeMinimumDis);
            Serial.print ("StartStep: ");
            Serial.println (StartStep);*/
        }
        long souldMoveSteps = TargetStep - CurrentStep;//距离差
        //SpeedUpStep表示加速结束的步数、SpeedDownStep表示减速开始的步数
        //路程较短情况，运动状态只有匀加速+匀减速  //if (CurrentStep < SpeedUpStep || CurrentStep > SpeedDownStep)
        if ( _speedChangeMinimumDis * 2 > abs(souldMoveSteps))
        {
          SpeedUpStep =  souldMoveSteps / 2 + CurrentStep;
          SpeedDownStep = SpeedUpStep;
        }
        //路程较长情况，运动状态有匀加速+匀速+匀减速
        else
        {
          if (CurrentStep < TargetStep)//正向
          {
            //SpeedUpStep和SpeedDownStep不是一个记录需要走的步数变量，而是与CurrentStep和TargetStep同一个事件轴的变量
            SpeedUpStep =  _speedChangeMinimumDis + CurrentStep;
            SpeedDownStep = TargetStep - _speedChangeMinimumDis;
          }
          else//反向
          {
            SpeedUpStep =  CurrentStep - _speedChangeMinimumDis;
            SpeedDownStep = TargetStep + _speedChangeMinimumDis;
          }
        }
        if (IsDebug)
        {
                    Serial.print ("SpeedUpStep: ");
                    Serial.println (SpeedUpStep);
                    Serial.print ("SpeedDownStep: ");
                    Serial.println (SpeedDownStep);
        }
      }

      if (IsRunToTarget() == false)
      {
        if (CanRun())
        {
          runStep(CurrentStep < target);
        }
      }

      return IsRunToTarget();
    }
    //目标驱动操作
    bool RunTo(long target, unsigned long stepTime)
    {
      TargetStepTime = stepTime;
      return RunTo(target);
    }

    //电机转动操作
    void RunCycle(bool dir,  unsigned long stepTime)
    {
      CurrentStepTime = stepTime;
      if (CanRun())
      {
        runStep( dir);
      }
    }

  private:
    long _speedChangeMinimumDis = 0;

    void calCulateSpeedChangeMinimunDis()
    {
      double v = 1000000.0 / TargetStepTime;//步/秒
      _speedChangeMinimumDis =  pow(v, 2) / 2 / Acceleration;//位移x=v^2/2a
    }

    //核心函数
    void runStep(bool dir)    //dir = CurrentStep < target
    {
      LastStepSt = micros();  //当前时间记录为上一步时间
      dir_bug = dir;
      digitalWrite(DirPin, dir);
      digitalWrite(PulPin, 1);
      delayMicroseconds(10);
      digitalWrite(PulPin, 0);
      if (IsDebug)
      {
        // Serial.print (CurrentStep);
        //Serial.print (",");
        //Serial.print ( v == 0 ? 1000000.0 / CurrentStepTime : v);
        Serial.print (",");
        Serial.println (CurrentStepTime);
      }
    }   
};

STEPPER  MyStepperA(51,49,47);
STEPPER  MyStepperB(41,43,45);
STEPPER  MyStepperC(32,30,28);
STEPPER  MyStepperD(22,24,26);


long targetA = 5000, targetB = 5000;

void setup()
{  
  Serial.begin(115200);
  Serial1.begin(38400);
  MyStepperA.SetAcceleration(1000);
  MyStepperB.SetAcceleration(1000);
  //MyStepperA.IsDebug = true;
  
}

unsigned long st = 0;
void f();void b();void l();void r();void q();void e();void a();void d();void s();

void loop()
{
 
  char buf;
  //while (Serial1.available() > 0){
   buf = Serial1.read();
   //Serial.print(buf);
   if(buf == 'f') 
   {
      while(1){
        f(); 
        if(Serial1.available() > 0) break;
        //Serial.print("f");
        }
      }
   else if(buf == 'b') 
   {
      while(1){
        b(); 
        if(Serial1.available() > 0) break;
        Serial.print("b");}
      }
   else if(buf == 'l') 
   {
      while(1){
        l(); 
        if(Serial1.available() > 0) break;
        Serial.print("b");}
    }
   else if(buf == 'r')
  {
     while(1){
        r(); 
        if(Serial1.available() > 0) break;
        Serial.print("b");}
  }
   else if(buf == 'q') 
   {
      while(1){
        q(); 
        if(Serial1.available() > 0) break;
        Serial.print("b");}
    }
   else if(buf == 'e') 
  {
      while(1){
        e(); 
        if(Serial1.available() > 0) break;
        Serial.print("b");}
  }
   else if(buf == 'a') 
   {
       while(1){
        a(); 
        if(Serial1.available() > 0) break;
        Serial.print("b");}
    }
   else if(buf == 'd') 
  {
      while(1){
        d(); 
        if(Serial1.available() > 0) break;
        Serial.print("b");}
  }
   else if(buf == 's') s();
   //Serial1.flush(); 
  //}
}   

/*
 //双电机差速起步测试
  //注意步时单位为微秒
  if (MyStepperA.RunTo(targetA, 500))
  {
    targetA = -targetA;
  }
  if (MyStepperB.RunTo(targetB, 500))
  {
    targetB = -targetB;
  }
}
*/

  /*
  Serial.print ("A_dir: ");
  Serial.println (MyStepperA.dir_bug);
  Serial.print ("B_dir: ");
  Serial.println (MyStepperB.dir_bug);
  */
  
/*
  if (millis() - st >= 10)
  {
    st = millis();
    Serial.print (1000000.0 / MyStepperB.CurrentStepTime);
    Serial.print (",");
    Serial.print (MyStepperB.CurrentStep);
    Serial.print (",");
    Serial.print (1000000.0 / MyStepperA.CurrentStepTime);
    Serial.print (",");
    Serial.println (MyStepperA.CurrentStep);
  }
*/

void f()
{
  MyStepperA.RunCycle(1,200);
  MyStepperB.RunCycle(0,200);
  MyStepperC.RunCycle(1,200);
  MyStepperD.RunCycle(0,200);
  }
void b()
{
  MyStepperA.RunCycle(0,200);
  MyStepperB.RunCycle(1,200);
  MyStepperC.RunCycle(0,200);
  MyStepperD.RunCycle(1,200);

}
void l()
{
  MyStepperA.RunCycle(0,200);
  MyStepperB.RunCycle(0,200);
  MyStepperC.RunCycle(0,200);
  MyStepperD.RunCycle(0,200);
}
void r()
{
  MyStepperA.RunCycle(1,200);
  MyStepperB.RunCycle(1,200);
  MyStepperC.RunCycle(1,200);
  MyStepperD.RunCycle(1,200);
}
void q()
{
  MyStepperA.RunCycle(1,700);
  MyStepperB.RunCycle(0,200);
  MyStepperC.RunCycle(1,700);
  MyStepperD.RunCycle(0,200);
}
void e()
{
  MyStepperA.RunCycle(1,200);
  MyStepperB.RunCycle(0,700);
  MyStepperC.RunCycle(1,200);
  MyStepperD.RunCycle(0,700);
}
void a()
{
  MyStepperA.RunCycle(0,700);
  MyStepperB.RunCycle(1,200);
  MyStepperC.RunCycle(0,700);
  MyStepperD.RunCycle(1,200);
}
void d()
{
  MyStepperA.RunCycle(0,200);
  MyStepperB.RunCycle(1,700);
  MyStepperC.RunCycle(0,200);
  MyStepperD.RunCycle(1,700);
}
void s()
{
  MyStepperA.RunCycle(0,0);
  MyStepperB.RunCycle(0,0);
  }
