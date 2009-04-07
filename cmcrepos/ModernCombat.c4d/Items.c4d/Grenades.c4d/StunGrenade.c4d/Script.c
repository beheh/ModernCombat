/*-- Blendgranate --*/

#strict
#include NADE

public func FuseTime(){return(2*35);}
public func Color(){return(RGB(0,128,255));}
public func ContainedDamage(){return(20);}

func HitSound()
{
  Sound("SGRN_Hit*.ogg");
}

public func Fused()
{
  Sound("STUN_Fused.ogg");

  for(var obj in FindObjects(Find_OCF(OCF_CrewMember),Find_Distance(200)))
  {
    var intensity = ((200-ObjectDistance(this(),obj))*470/200)/2;
    
    if(obj->~IsClonk())
    {
      if(!PathFree(GetX(),GetY(),GetX(obj),GetY(obj)-8))//Speziell für Clonkaugen. >,<
        continue;

      if(((GetDir(obj) == DIR_Left) && (GetX(obj) < GetX())) ||
         ((GetDir(obj) == DIR_Right) && (GetX(obj) > GetX())))
      {
        intensity = Max(intensity,255);
      }
    }
    else
      if(!PathFree(GetX(),GetY(),GetX(obj),GetY(obj))) continue;

    AddEffect("IntFlashbang",obj,10,1,0,GetID(), intensity); 
  }
  
  Sparks(30,RGB(255,128));
  CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  CastParticles("PxSpark",10,40,0,0,60,120,RGBa(255,255,255,70),RGBa(255,255,255,70));
  AddLightFlash(50,0,0,RGB(255,255,255),this());
  //BlastObjects(GetX(),GetY(),10);
  FadeOut();
}

//intensity: 0-100-200 ...
public func FxIntFlashbangStart(object pTarget, int iEffectNumber, int iTemp, intensity)
{
  if(!intensity) return(-1);
  EffectVar(0,pTarget,iEffectNumber) = intensity;

  var a = BoundBy(255-(intensity*255/100),0,255);
  var flash = ScreenRGB3(pTarget,RGBa(255,255,255,a));
  if(!flash)
    return(-1);
  else
    EffectVar(1,pTarget,iEffectNumber) = flash;
}

public func FxIntFlashbangTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var i = EffectVar(0,pTarget,iEffectNumber)--;
  if(i <= 0) return(-1);
  if(!EffectVar(1,pTarget,iEffectNumber)) return(-1);
  
  var a = BoundBy(255-(i*255/100),0,255);
  EffectVar(1,pTarget,iEffectNumber)->Set(pTarget,RGBa(255,255,255,a));
  
  Message("<c %x>•</c>",pTarget,RGBa(255,255,255,a));
}

public func FxIntFlashbangStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(1,pTarget,iEffectNumber))
    EffectVar(1,pTarget,iEffectNumber)->RemoveObject();
}