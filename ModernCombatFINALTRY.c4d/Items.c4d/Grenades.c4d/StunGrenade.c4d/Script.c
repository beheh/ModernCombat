/*-- Blendgranate --*/

#strict 2
#include NADE

public func Color(){return RGB(0,128,255);}
public func ContainedDamage(){return 20;}

func HitSound()
{
  Sound("SGRN_Hit*.ogg");
}

public func Fused()
{
  for(var obj in FindObjects(Find_OCF(OCF_CrewMember),Find_Distance(200)))
  {
    var intensity = ((200-ObjectDistance(this,obj))*470/200)/2;

    if(obj->~IsClonk())
    {
      if(!PathFree(GetX(),GetY(),GetX(obj),GetY(obj)-8))//Speziell für Clonkaugen. >,<
        continue;
      if(((GetDir(obj) == DIR_Left) && (GetX() < GetX(obj))) || ((GetDir(obj) == DIR_Right) && (GetX(obj) > GetX(obj))))
        intensity = Max(intensity,255);
    }
    else
    {
      if(!PathFree(GetX(),GetY(),GetX(obj),GetY(obj))) continue;
    }
    AddEffect("IntFlashbang",obj,10,1,0,GetID(), intensity); 
  }

  //Effekte
  Sound("STUN_Fused.ogg");
  Sparks(30,RGB(255,128));
  CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  CastParticles("PxSpark",10,40,0,0,60,120,RGBa(255,255,255,70),RGBa(255,255,255,70));
  AddLightFlash(50,0,0,RGB(255,255,128),this);
  FadeOut();
}


/* Blendeffekt */
//intensity: 0-100-200 ...
public func FxIntFlashbangStart(object pTarget, int iEffectNumber, int iTemp, intensity)
{
  if(!intensity) return -1;
  EffectVar(0,pTarget,iEffectNumber) = intensity;

  var a = BoundBy(255-(intensity*255/100),0,255);
  var flash = ScreenRGB3(pTarget,RGBa(255,255,255,a));
  if(!flash)
    return -1;
  else
    EffectVar(1,pTarget,iEffectNumber) = flash;
}

public func FxIntFlashbangTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var i = EffectVar(0,pTarget,iEffectNumber)--;
  if(i <= 0) return -1;
  if(!EffectVar(1,pTarget,iEffectNumber)) return -1;
  
  var a = BoundBy(255-(i*255/100),0,255);
  EffectVar(1,pTarget,iEffectNumber)->Set(pTarget,RGBa(255,255,255,a));
  
  var val, num, pCursor;
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    pCursor = GetCursor(GetPlayerByIndex(i))->~GetRealCursor();
    if(!pCursor) pCursor = GetCursor(GetPlayerByIndex(i));
    num = GetEffect("IntFlashbang", pCursor);
    if(num) {
      val = 255-BoundBy((EffectVar(0,pCursor,num)-1)*255/100,0,255);
      if(255-a < val) val = 255-a;
    }
    else {
      val = 255-a;
    }
    PlayerMessage(GetPlayerByIndex(i), "<c %x>•</c>", pTarget, RGBa(255,255,255,BoundBy(val, 1, 254)));
  }
}

public func FxIntFlashbangStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(1,pTarget,iEffectNumber))
    EffectVar(1,pTarget,iEffectNumber)->RemoveObject();
}
