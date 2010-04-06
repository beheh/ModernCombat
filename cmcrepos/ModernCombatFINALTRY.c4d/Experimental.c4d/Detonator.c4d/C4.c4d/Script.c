/*-- C4 Ladung --*/

#strict 2

local fuse, active, thrown;


/* Initalisierung */

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
//else
  //AddEffect("Check", this(), 200, 1, this(), C4EX);
  
  //Effekte
  Sound("C4PA_Ignition.ogg");
  CreateParticle("PSpark",0,0,0,0,60,RGBa(255,0,0,0),this());
}

/* Prüfungseffekt */
/*
public func FxCheckStart(pTarget, iNo, iTemp)
{
  if(iTemp)
    return -1;
}

public func FxCheckTimer(pTarget, iNo, iTime)
{
  var obj;
  if(obj = FindObject2(Find_AtPoint(), Find_Func("IsBulletTarget", GetID()), Find_NoContainer(),Find_Not(Find_OCF(OCF_Alive))))
  {
   if(GetAction()!="Attaching")
   {
     Sound("C4EX_Attach.ogg");
     SetRDir(0);
     SetAction("Attaching");
   }
   SetPosition((AbsX()*-1)+GetXDir(obj),(AbsY()*-1)+GetYDir(obj));
  }
  else
    if(GetAction()!="Idle")
    {
      SetRDir(RandomX(-20,20));
      SetAction("Idle");
    }
}
*/
/* Zündung */

public func Trigger()
{
  Sound("C4EX_Ignition.ogg");
  CreateParticle("PSpark",0,0,0,0,60,RGBa(255,0,0,0),this());
  ScheduleCall(0, "BlowUp", 10);
}

/* Explosion */

public func BlastRadius() {return 50;}

public func BlowUp()
{
  if(GBackLiquid())
   Sound("C4EX_WaterDetonation.ogg");
  else
   Sound("C4EX_Detonation*.ogg");

  //Effekte
  var helper = CreateObject(TIM1,0,0,-1);
  AddEffect("IntShockWave",helper,10,1,0,GetID()); 
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));
  
  //Extraschaden für Strukturen
  for(var obj in FindObjects(Find_Distance(50), Find_Category(C4D_Structure | C4D_Vehicle)))
   DoDmg(BoundBy(InvertA1(ObjectDistance(obj), 100),0,60), DMG_Explosion, obj, 0, GetOwner());

  Explode(BlastRadius());
  RemoveObject();
}

/* Außeneinwirkung */

public func IsBulletTarget(ID)
{
  if(ID == C4EX)
    return false;
  return true;
}

public func OnDmg(int iDamage, int iType)
{
  if((iType == DMG_Fire || iType == DMG_Explosion) && iDamage > 10)
   Trigger();
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
  pTarget->CreateParticle("ShockWave",0,0,0,0,iEffectTime*(10*(C4EX->BlastRadius()*3/2))/5,RGB(255,255,128));
  if(iEffectTime >= 5) return -1;
}

public func FxIntShockWaveStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  RemoveObject(pTarget);
}
