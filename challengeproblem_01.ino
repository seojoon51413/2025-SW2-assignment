int led = 8 ; //7번 핀에 회로를 꼽고 힘을 잘못 줘서 고장나 버려서 불가피하게 8번으로 하게됐습니다 죄송합니다.
double period = 100; //주기
double duty =0 ;//초기 밝기
double count = 0.01 ; //duty의 증가값
double light_time = 500000 ; //0.5초(최대 밝기인 순간)
double bright_time=0;//pwm 구현
double black_time=0;//pwm구현
double c;
int k =0;
double set_period(double period ){
  //period가 100,1000,10000이 되도록.
  if (k==0){
    period = 100;
  }else if(k==1){
    period=1000;
  }else if (k==2){
    period=10000;
  }
  return period;
  }
double set_duty(double duty){
  //duty가 최대 밝기가 되면 다시 줄어들고 최소 밝기가 되면 다시 증가 하도록.
  duty=duty + count;
  if (duty >=1){
    duty=1.0;
    count=-count;
  }
  if (duty <= 0){
    duty=0.0;
    count=-count;
  }
  return duty;
  }
void setup() {
  pinMode(led,OUTPUT); 
}

void loop() {
  period = set_period(period); //period 결정
  int period_count=light_time/period;//0.5초까지 주기가 몇번 반복하는지
  c = period_count/100; //같은 duty가 어느 만큼의 시간을 반복해야하는지 ( duty는 정수 값 이기 때문에)
  for (int i =0 ; i<=1000000; i=i+period){ 
    //1초가 될 때까지(0.5초까지 천천히 밝아졌다가 다시 1초까지 서서히 꺼짐)
    duty=set_duty(duty);
    bright_time=period*duty;//pwm구현
    black_time=period-bright_time; //pwm 구현
    for (int d=0 ; d<=c ;d++){
      digitalWrite(led,HIGH);
      delayMicroseconds(bright_time);//pwm 구현
      digitalWrite(led,LOW);
      delayMicroseconds(black_time); //pwm 구현
    }
    
  }
  k=(k+1)%3;
}

