#strict 2

/* Unstuck-Effect */
/*Effekt der Objekte augenfreundlich aus Materialien herauszieht.*/
global func FxIntUnstuck4KStart(object pTarget, int iEffectNumber, int iTemp,iXDir,iYDir)
{
  if(!iXDir && !iYDir)
  {
    for(var j = 10;j < 60;j+=10)
    {
      for(var i = -90;i < 360-90;i+=45)//In 45° Schritten testen.
      {
        iXDir = Cos(i,j);
        iYDir = Sin(i,j);
        if(!GBackSolid(iXDir,iYDir))
        {
          Normalize4K(iXDir,iYDir);
          EffectVar (0,pTarget,iEffectNumber) = iXDir;
          EffectVar (1,pTarget,iEffectNumber) = iYDir;
          return 1;
        }
      }
    }
    
    EffectVar (0,pTarget,iEffectNumber) =  0;
    EffectVar (1,pTarget,iEffectNumber) = -1;
  }
  else
  {
    EffectVar (0,pTarget,iEffectNumber) = iXDir;
    EffectVar (1,pTarget,iEffectNumber) = iYDir;
  }
  return 1;
}

global func FxIntUnstuck4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!Stuck(pTarget))
    return -1;

  if(iEffectTime > 10)
    return -1;

  SetPosition(GetX(pTarget)  -  EffectVar (0,pTarget,iEffectNumber),
              GetY(pTarget)  -  EffectVar (1,pTarget,iEffectNumber));

  return 0;
}

global func AutoUnstuck(pTarget,iXDir,iYDir)
{
  AutoUnstuck4K(pTarget,iXDir,iYDir);
}

global func AutoUnstuck4K(pTarget,iXDir,iYDir)
{
  if(!GetEffect("IntUnstuck4K",pTarget))
    AddEffect("IntUnstuck4K",pTarget,1,1,pTarget,0,iXDir,iYDir); 
}


/* Rauch-Effect */
/*Effekt der Wraks schön rauchen und brennen lässt.*/
global func FxIntWreckSmoke4KStart(object pTarget, int iEffectNumber, int iTemp,iP,iX,iY,bF)
{
  EffectVar (0,pTarget,iEffectNumber) = iP;
  EffectVar (1,pTarget,iEffectNumber) = iX;
  EffectVar (2,pTarget,iEffectNumber) = iY;
  EffectVar (3,pTarget,iEffectNumber) = bF;
  return 1;
}

global func FxIntWreckSmoke4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!pTarget)
    return -1;

  var iP = EffectVar (0,pTarget,iEffectNumber);
  var iX = EffectVar (1,pTarget,iEffectNumber);
  var iY = EffectVar (2,pTarget,iEffectNumber);
  
  GetOffset4K(iX,iY);
  
  var vel = Abs(GetXDir(pTarget))+Abs(GetYDir(pTarget));
  var alpha = Max(0,30-vel);
  var black = 40 + Random(40);
  
  if(EffectVar(3,pTarget,iEffectNumber))
    CreateParticle("Thrust",iX,iY,GetXDir()/2,GetYDir()/2,iP + Random(iP/2) + vel/3 + 40,RGBa(255,200,200,60));
  
  CreateParticle("Smoke3",iX,iY,RandomX(-1,1),-Random(5),iP + Random(iP/2) + vel/3,RGBa(black,black,black,alpha)); 

  return 0;
}

global func AddSmokeEffect4K(iStrength,iX,iY,bBurning,pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return 0;
  AddEffect ("IntWreckSmoke4K",pTarget,10,2,pTarget,0,iStrength,iX,iY,bBurning);
  return 1;
}


/*SpeedSecure®-Effekt*/
//Das Objekt kann bei hohen Geschwindigkeiten nicht eingesammelt werden.

global func FxIntSpeedSecure4KStart(object pTarget, int iEffectNumber, int iTemp,max_speed)
{
  EffectVar (0,pTarget,iEffectNumber) = max_speed;
  return 1;
}

global func FxIntSpeedSecure4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(Distance(GetXDir(pTarget),GetYDir(pTarget)) > EffectVar (0,pTarget,iEffectNumber))
    SetCategory(C4D_Vehicle,pTarget);
  else
    SetCategory(C4D_Object,pTarget);
  return 0;
}

global func FxIntSpeedSecure4KStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  SetCategory(C4D_Object,pTarget);
  return -1;
}

global func SetSpeedSecure(iMaxSpeed,pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  
  if(!iMaxSpeed) iMaxSpeed = 20;
  
  AddEffect ("IntSpeedSecure4K",pTarget,200,5,pTarget,0,iMaxSpeed); 
}

/* Neue Aus- und Einfade-Effekte (Original gabs im Hazard-Pack.) */
global func FxFadeOut4KStart(target, no, temp, speed)
{
  EffectVar(0, target, no) = 0;
  EffectVar(1, target, no) = Max(speed,1);
  if(!GetClrModulation(target))
    EffectVar(2, target, no) = RGB(255,255,255);
  else
    EffectVar(2, target, no) = GetClrModulation(target);
}

global func FxFadeOut4KTimer(target, no)
{
  EffectVar(0, target, no) = BoundBy(EffectVar(0, target, no) + EffectVar(1, target, no), 0, 255);
  SetClrModulation(SetRGBaValue(EffectVar(2, target, no),EffectVar(0, target, no),0), target);
  
  if(EffectVar(0, target, no) >= 255)
  {
    RemoveObject(target);
    return -1;
  }
}

global func FadeOut4K(int iSpeed, object pObject)
{
  if(!pObject) pObject = this;
  if(!pObject) return ;

  if(GetEffect("*FadeOut*", pObject)) return ;
  return AddEffect("FadeOut4K", pObject, 101, 2,0,0,iSpeed);
}

global func FxFadeIn4KStart(target, no, temp, speed)
{
  EffectVar(0, target, no) = 255;
  EffectVar(1, target, no) = Max(speed,1);
  if(!GetClrModulation(target))
    EffectVar(2, target, no) = RGB(255,255,255);
  else
    EffectVar(2, target, no) = GetClrModulation(target);
  
  SetClrModulation(SetRGBaValue(EffectVar(2, target, no),255,0), target);
}

global func FxFadeIn4KTimer(target, no)
{
  EffectVar(0, target, no) = BoundBy(EffectVar(0, target, no) - EffectVar(1, target, no), 0, 255);
  SetClrModulation(RGBa(255,255,255, EffectVar(0, target, no)), target);
  SetClrModulation(SetRGBaValue(EffectVar(2, target, no),EffectVar(0, target, no),0), target);
  
  if(EffectVar(0, target, no) <= 0)
  {
    return -1;
  }
}

global func FadeIn4K(int iSpeed, object pObject)
{
  if(!pObject) pObject = this;
  if(!pObject) return ;

  if(GetEffect("*FadeIn*", pObject)) return ;
  return AddEffect("FadeIn4K", pObject, 101, 2,0,0,iSpeed);
}


/* TimedMessage-Effekt */
//Wie Message(), nur das ein Parameter die Anzeigezeit festlegt.

global func FxIntTimedMessage4KStart(object pTarget, int iEffectNumber, int iTemp,szMessage)
{
  Message("@%s",pTarget,szMessage);
}

global func FxIntTimedMessage4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  Message(" ",pTarget);
  return -1;
}

global func FxIntTimedMessage4KStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //...
}

global func TimedMessage4K(string szMessage, object pTarget, int iTime)
{
  if(!iTime) iTime = 35*3;
  
  var iEffect;
  var i = GetEffectCount("IntTimedMessage4K",pTarget);
  while(i--)
    if(iEffect = GetEffect("IntTimedMessage4K",pTarget,i))
      RemoveEffect(0, pTarget, iEffect);
  
  AddEffect("IntTimedMessage4K",pTarget,10,iTime,pTarget,0,Format(szMessage,...)); 
}

/* TimedPlayerMessage-Effekt */
//Wie Message(), nur das ein Parameter die Anzeigezeit festlegt.

global func FxIntTimedPlayerMessage4KStart(object pTarget, int iEffectNumber, int iTemp,szMessage,iPlr)
{
  PlayerMessage(iPlr,"@%s",pTarget,szMessage);
  EffectVar(0,pTarget,iEffectNumber) = iPlr; 
}

global func FxIntTimedPlayerMessage4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  PlayerMessage(EffectVar(0,pTarget,iEffectNumber)," ",pTarget);
  return -1;
}

global func FxIntTimedPlayerMessage4KStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //...
}

global func TimedPlayerMessage4K(int iPlr, string szMessage, object pTarget, int iTime)
{
  if(!iTime) iTime = 35*3;
  
  var iEffect;
  var i = GetEffectCount("IntTimedPlayerMessage4K",pTarget);
  while(i--)
    if(iEffect = GetEffect("IntTimedPlayerMessage4K",pTarget,i))
      RemoveEffect(0, pTarget, iEffect);
  
  AddEffect("IntTimedPlayerMessage4K",pTarget,10,iTime,pTarget,0,Format(szMessage,...),iPlr); 
}
