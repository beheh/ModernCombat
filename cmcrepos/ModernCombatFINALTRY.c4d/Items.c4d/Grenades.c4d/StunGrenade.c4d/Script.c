/*-- Blendgranate --*/

#strict 2
#include NADE

local FadingOut;

public func Color()		{return RGB(0,128,255);}	//Farbe
public func ContainedDamage()	{return 20;}			//Schaden bei Detonation innerhalb eines Objekts
public func IsFadingOut()	{return FadingOut;}


/* Aktivierung */

public func Fuse()
{
  Sound("STUN_Activate.ogg");
  SetGraphics("Active");
  return AddEffect ("IntFuse",this,200,1,this);
}

public func Fused()
{
  //Zu blendende Objekte suchen
  for(var obj in FindObjects(Find_OCF(OCF_CrewMember),Find_Distance(250)))
  {
    //Ziel verschachtelt: Auslassen, es sei denn, der Container ist ein Helikopter
    if(Contained(obj) && !Contained(obj)->~IsHelicopter())
      continue;

    //Intensit�t errechnen
    var intensity = ((270-ObjectDistance(this,obj))*470/250)/2;

    //Ziel ein Clonk?
    if(obj->~IsClonk())
    {
      //Freie Sicht zum Clonk?
      if(!PathFree(GetX(),GetY(),GetX(obj),GetY(obj)-8))
        continue;

      //Clonk freundlich? Intensit�t halbieren
      if(!Hostile(GetOwner(obj), GetOwner(GetUser())))
        intensity = intensity/2;
    }
    else
    {
      if(!PathFree(GetX(),GetY(),GetX(obj),GetY(obj))) continue;
    }
    AddEffect("IntFlashbang",obj,1,1,0,GetID(), intensity, GetOwner(GetUser())); 
  }

  //Effekte
  Sound("STUN_Fused.ogg");
  Sparks(30,RGB(255,128));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12,40,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  CastParticles("PxSpark",10,40,0,0,60,120,RGBa(255,255,255,70),RGBa(255,255,255,70));
  for(var i = 0; i <= 3; i++) AddLightFlash(250,0,0,RGB(255,255,255),this);

  //Verschwinden
  FadingOut = true;
  if(OnFire())
    RemoveObject();
  else
    FadeOut();
}

/* Blendeffekt */

public func FxIntFlashbangStart(object pTarget, int iEffectNumber, int iTemp, intensity, owner)
{
  //Keine Intensit�t?
  if(!intensity) return -1;

  EffectVar(0,pTarget,iEffectNumber) = intensity;
  EffectVar(2,pTarget,iEffectNumber) = owner;	//Zur Punkteberechnung

  //Blendung stark genug? Dann auch h�rbar
  if(intensity > 38) Sound("STUN_Bang.ogg", false, pTarget, 100, GetOwner(pTarget)+1);

  //St�rke berechnen und Blendung erstellen
  var a = BoundBy(255-(intensity*255/100),0,255);

  //Ziel bereits geblendet? Hinzuaddieren
  if(GetEffectCount("IntFlashbang", pTarget) > 1)
  {
    for(var i = 0; i < GetEffectCount("IntFlashbang", pTarget); i++)
    {
       var nr = GetEffect("IntFlashbang", pTarget, i);
      if(nr != iEffectNumber)
      {
        EffectVar(0, pTarget, nr) = BoundBy(EffectVar(0, pTarget, nr) + intensity, 0, 255);
        return -1;
      }
    }
  }

  var flash = ScreenRGB(pTarget,RGBa(255,255,255,a), 0, 0, false, SR4K_LayerLight);
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
  EffectVar(1,pTarget,iEffectNumber)->SetAlpha(a);
  
  var val, num, pCursor, c, flag;
  if(!Contained())
  {
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      pCursor = GetCursor(GetPlayerByIndex(i))->~GetRealCursor();
      if(!pCursor) pCursor = GetCursor(GetPlayerByIndex(i));
      if(!pCursor) continue;
      if(Contained(pCursor)) continue;
      num = GetEffect("IntFlashbang", pCursor);
      if(num)
      {
        val = 255-BoundBy((EffectVar(0,pCursor,num)-1)*255/100,0,255);
        if(255-a < val) val = 255-a;
      }
      else
      {
        val = 255-a;
      }
      flag = 0;
      if(c != 0) flag = MSG_Multiple;
      CustomMessage(Format("<c %x>{{SM07}}</c>", RGBa(255,255,255,BoundBy(val, 1, 254))), pTarget, GetPlayerByIndex(i), 0, 0, 0, 0, 0, flag); 
      c++;
    }
  }
  else
  {
    Message("@", pTarget); 
  }
}

public func FxIntFlashbangStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(1,pTarget,iEffectNumber))
    RemoveObject(EffectVar(1,pTarget,iEffectNumber));
}

/* Schaden */

protected func Damage(int iChange) 
{
  //Kein Schaden nehmen wenn gehalten und eventuelles Feuer l�schen
  if(Contained())
  {
    if(OnFire())
    {
      Extinguish();
      return;
    }
    else
    return;
  }

  //Ansonsten Z�ndung durch Schaden
  if(GetDamage() < MaxDamage() || activated) return;
    Fused();

  //Z�ndung
  Fused();

  //Verschwinden
  RemoveObject();
}

/* Aufprall */

func HitSound()
{
  Sound("STUN_Hit*.ogg");
}