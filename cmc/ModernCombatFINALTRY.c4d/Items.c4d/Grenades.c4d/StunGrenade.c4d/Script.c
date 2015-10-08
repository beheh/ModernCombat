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
    //Ziel verschachtelt aber nicht in einem Helikopter oder Ziel kein Clonk: Abbruch
    if(Contained(obj) && !Contained(obj)->~IsHelicopter())
      continue;

    //Entfernung zum Clonk berechnen
    if(ObjectDistance(this, obj) > 50)
    {
      //Weiter als 50px entfernt: Nur blenden wenn freies Sichtfeld
      if(!PathFree(GetX(),GetY(),GetX(obj),GetY(obj)-8))
        continue;
    }

    //Blendwert errechnen
    var intensity = ((400-ObjectDistance(this,obj))*600/250)/2;
    //Blendwert 0 oder weniger: Abbruch
    if(intensity <= 0)
      continue;
    //Clonk ist verbündet: Blendwert halbieren
    if(!Hostile(GetOwner(obj), GetOwner(GetUser())))
      intensity = intensity/2;

    //Blendeffekt am Clonk erstellen oder vorhandenen aktualisieren
    var effect = GetEffect("IntFlashbang", obj);
    if(!effect)
      AddEffect("IntFlashbang",obj,10,1,0,GetID(), intensity, GetOwner(GetUser()));
    else
      EffectVar(0, obj, effect) += intensity;

    //Blendung stark genug: Soundeffekte am Clonk
    if(intensity > 38)
    {
      if(!GetEffect("SoundDelay", obj))
        Sound("ClonkSmallPain*.ogg", 0, obj, 100, GetOwner(obj)+1);
      Sound("STUN_Bang*.ogg", false, obj, 100, GetOwner(obj)+1);
      Sound("STUN_EarRinging*.ogg", false, obj, 100, GetOwner(obj)+1);
    }
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

public func FxIntFlashbangStart(object pTarget, int iEffectNumber, int iTemp, intensity)
{
  if(iTemp)
    return;

  //Blendwert übernehmen, bei 0 abbrechen
  if(!intensity)
    return -1;
  EffectVar(0,pTarget,iEffectNumber) = intensity;

  //Blendwert in Alphawert umwandeln
  var a = BoundBy(intensity,0,255);
  //Blendobjekt erstellen und speichern sowie Alphawert setzen
  var flash = ScreenRGB(pTarget,RGB(255,255,255), 0, 0, false, SR4K_LayerLight);
  flash->SetAlpha(255-a);
  if(!flash)
    return -1;
  else
    EffectVar(1,pTarget,iEffectNumber) = flash;
}

public func FxIntFlashbangTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Kein Blendobjekt vorhanden: Abbruch
  var rgb = EffectVar(1,pTarget,iEffectNumber);
  if(!rgb)
    return -1;

  //Aktuellen Blendwert ermitteln
  var i;
  if(--EffectVar(0, pTarget, iEffectNumber) >= 100)
    i = EffectVar(0, pTarget, iEffectNumber);
  else
    //Aktuelle Intensität anhand des ScreenRGB-Alphawerts ermitteln
    i = ((255-rgb->~GetAlpha()) * 100 / 255)-1;

  if(i <= 0) return -1;

  var a = BoundBy(255-(i*255/100)-1,0,255);
  rgb->SetAlpha(a);

  var val, num, pCursor, c, flag;
  if(!Contained())
  {
    var a = rgb->~GetAlpha(), c;
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      var pCursor = GetCursor(GetPlayerByIndex(i))->~GetRealCursor();
      if(!pCursor && !(pCursor = GetCursor(GetPlayerByIndex(i)))) 
        continue;

      if(Contained(pCursor))
        continue;

      var flash = GetScreenRGB(GetPlayerByIndex(i), SR4K_LayerLight, pCursor);
      var val = 0;

      if(flash)
      {
        val = flash->~GetAlpha();

        if(val > 127)
          val = 255-val;
      }
      else
        val = 255-a;

      var flag = 0;
      if(c != 0)
        flag = MSG_Multiple;

      CustomMessage(Format("<c %x>{{SM07}}</c>", RGBa(255,255,255,BoundBy(val, 1, 254))), pTarget, GetPlayerByIndex(i), 0, 0, 0, 0, 0, flag); 
      c++;
    }
  }
  else
    Message("@", pTarget);
}

public func FxIntFlashbangStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(1,pTarget,iEffectNumber))
    RemoveObject(EffectVar(1,pTarget,iEffectNumber));
}

/* Schaden */

protected func Damage(int iChange)
{
  //Kein Schaden nehmen wenn gehalten und eventuelles Feuer löschen
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

  //Ansonsten Zündung durch Schaden
  if(GetDamage() < MaxDamage() || activated) return;
    Fused();

  //Zündung
  Fused();

  //Verschwinden
  RemoveObject();
}

/* Aufprall */

func HitSound()
{
  Sound("STUN_Hit*.ogg");
}