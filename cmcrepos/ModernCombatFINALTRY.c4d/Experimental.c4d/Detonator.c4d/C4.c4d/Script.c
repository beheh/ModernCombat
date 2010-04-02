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
  else
    AddEffect("Check", this(), 200, 1, this(), C4EX);
  
  //Effekte
  Sound("C4PA_Ignition.ogg");
  CreateParticle("PSpark",0,0,0,0,60,RGBa(255,0,0,0),this());
}

/* Prüfungseffekt */

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
  if(obj = FindObject2(Find_AtPoint(), Find_Func("IsBulletTarget", GetID()), Find_NoContainer(),Find_Not(Find_OCF(OCF_Alive))))
  {
   Sound("C4EX_Attach.ogg");
   SetRDir(0);
   SetAction("Attaching", obj);

   //Nahesten Vertex finden
   var nearest = [0,0,500], dist;
   for(var i = 0; i < GetVertexNum(obj); i++)
    for(var j = 0; j < GetVertexNum(pTarget); j++)
      if(dist = Distance(GetX(obj)+GetVertex(i,0,obj),GetY(obj)+GetVertex(i,1,obj),
                 GetX(pTarget)+GetVertex(0,0,pTarget),GetY(pTarget)+GetVertex(0,1,pTarget))
                 < nearest[2])
        {
         nearest[0] = i;
         nearest[1] = j;
         nearest[2] = dist;
        }
    SetActionData(256*nearest[j] + nearest[i], pTarget);
  }
}

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
   DoDmg(BoundBy(InvertA1(ObjectDistance(obj), 60),0,60), DMG_Explosion, obj, 0, GetOwner());

  Explode(BlastRadius());
}

/* Außeneinwirkung */

public func OnDmg(int iDamage, int iType)
{
  if(iType == DMG_Fire || iType == DMG_Explosion)
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