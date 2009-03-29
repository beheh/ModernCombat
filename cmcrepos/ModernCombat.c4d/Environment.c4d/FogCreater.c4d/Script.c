/*-- Nebliger Wind --*/

#strict

local vel;

protected func Initialize()
{
  SetPosition();//In die Ecke mit dir!
  
  vel = 30;
  
  //Startnebel
  for(var i = LandscapeWidth()/10; i > 0; i--)
    DoFog(i*10);
}

protected func Timer()
{
  DoFog();
  
  if(!Random(100))
    if(!GetEffect("IntPush",this()))
      AddEffect ("IntPush", this(), 50, 1, this(), GetID(), 35*(3+Random(10)));
}

protected func DoFog(x)
{
  //atmosphärischen Nebel an den Kartenrändern machen
  var grey = 100+Random(155);
  
  var obj = this();
  if(!Random(3))
    obj = 0;
    
  CreateParticle ("Fog", x, Random(LandscapeHeight()), vel+Random(15), 0, 300+Random(600), RGBa(grey,grey,grey,Random(100)), obj, false); 
}

public func FxIntPushStart(object pTarget, int iEffectNumber, int iTemp, duration)
{
  EffectVar(0,pTarget,iEffectNumber) = duration;
}

public func FxIntPushTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var duration = EffectVar(0,pTarget,iEffectNumber);
  
  if(iEffectTime >= duration) return(-1);
  
  if(iEffectTime < duration/3)
  {
    vel+=2;
    PushParticles("Fog",+2);
  }
  else
  {
    vel--;
    PushParticles("Fog",-1);
  }
}

public func FxIntPushStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  vel = 30;
}