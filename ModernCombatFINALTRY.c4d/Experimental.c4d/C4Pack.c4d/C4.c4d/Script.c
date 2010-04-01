/*-- C4 Charge --*/

#strict 2

local fuse, active, thrown;

public func Initialize()
{
  active = false;
  thrown = false;
  fuse = 0;
}

public func GetPacket(pObj)
{
  return pObj == fuse;
}

public func SetActive(object pCaller)
{
  if(!pCaller)
    return;
    
  fuse = pCaller;
  active = true;
  
  if(!GetXDir() && !GetYDir())
    SetClrModulation(RGBa(255,255,255,100));
  else
    AddEffect("Check", this(), 200, 1, this(), C4__);
  
  //Effekte
  Sound("Bip"); //Vielleicht eigenen C4 Aktivierungssound?
  CreateParticle("PSpark",0,0,0,0,60,RGBa(255,0,0,0),this());
}

public func FxCheckStart(pTarget, iNo, iTemp)
{
  if(iTemp)
    return -1;
}

public func FxCheckTimer(pTarget, iNo, iTime)
{
  if(!GetXDir(pTarget) && !GetYDir(pTarget))
    return;
  
  var obj;
  if(obj = FindObject2(Find_AtPoint(GetX(pTarget),GetY(pTarget)), Find_Func("IsBulletTarget", GetID()), Find_NoContainer()))
    SetAction("Attaching", obj);
}

public func Trigger()
{
  Sound("Bip"); //Eigener C4 Zündsound
  
  CreateParticle("PSpark",0,0,0,0,60,RGBa(255,0,0,0),this());
  ScheduleCall(0, "BlowUp", 5);
}

public func BlastRadius() { return 50; }

public func BlowUp()
{
  //Sound(...)   Eigener Blastsound
  
  //Effekte vielleicht Umgestaltung?
  var helper = CreateObject(TIM1,0,0,-1);
  AddEffect("IntShockWave",helper,10,1,0,GetID()); 
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));
  
  //Extraschaden für alles Unorganisches
  for(var obj in FindObjects(Find_Distance(50), Find_Category(C4D_Structure | C4D_Vehicle)))
    DoDmg(BoundBy(InvertA1(ObjectDistance(obj), 60),0,60), DMG_Explosion, obj, 0, GetOwner());
  
  Explode(BlastRadius());
}

public func OnDmg(int iDamage, int iType)
{
  if((iType == DMG_Fire || iType == DMG_Explosion) && iDamage > 15)
    BlowUp();
}

func Incineration()
{
  BlowUp();
}


/* Schockwelle */

public func FxIntShockWaveStart(object pTarget, int iEffectNumber, int iTemp)
{
  //...
}

public func FxIntShockWaveTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->CreateParticle("ShockWave",0,0,0,0,iEffectTime*(10*(C4__->BlastRadius()*3/2))/5,RGB(255,255,128));
  if(iEffectTime >= 5) return -1;
}

public func FxIntShockWaveStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  RemoveObject(pTarget);
}
  
  
  
