/*-- Lichter --*/

#strict 2

local light,count,distance,left,right;


/* Initialisierung */

func Initialize()
{
  count = 5;//Lichterzahl
  distance = 20;//Pixelabstand zwischen den Lichtern
  left = true;
  right = true;
  return 1;
}

func SetType(bool bLeft,bool bRight)
{
  if(!bLeft && !bRight)
  {
    bLeft = true;
    bRight = true;
  }
  
  left = bLeft;
  right = bRight;
}

func GetLeft()			{return left;}
func GetRight()			{return right;}

func SetCount(int iCount)	{count = iCount;}
func GetCount()			{return count;}

func SetDistance(int iDist)	{distance = iDist;}
func GetDistance()		{return distance;}

/* Konfiguration */

func Set(int iCount, int iDist, bool bLeft, bool bRight, int iTimer)
{
  count = iCount;
  SetType(bLeft,bRight);
  if(!iDist) iDist = 15;
  distance = iDist;
  count = BoundBy(count,0,distance);
  if(!iTimer) iTimer = 35;

  RemoveEffect("IntOLights");
  AddEffect("IntOLights",this,50,iTimer,this,GetID()); 
}

/* Timer */

func Timer()
{
  light--;
  if(light <= 0) light=count;

  if(left)
    CreateParticle ("NoGravSpark", -light*distance, 0, 0, 0, 20, RGBa(250,170,100,0));

  if(right)
    CreateParticle ("NoGravSpark", +light*distance, 0, 0, 0, 20, RGBa(250,170,100,0)); 
}

/* Lichteffekt */

public func FxIntOLightsStart(object pTarget, int iEffectNumber, int iTemp)
{
  return 1;
}

public func FxIntOLightsTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  Timer();
  return 0;
}

public func FxIntOLightsStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  return -1;
}