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

  return AddEffect("IntFuse",this,200,1,this);
}

public func Fused()
{
  //Zu blendende Objekte suchen
  for(var obj in FindObjects(Find_OCF(OCF_CrewMember),Find_Distance(250)))
  {
    //Ziel verschachtelt aber nicht in einem Helikopter oder Ziel kein Clonk: Abbruch
    if(Contained(obj) && !Contained(obj)->~IsHelicopter()  || !obj->~IsClonk())
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
      AddEffect("IntFlashbang",obj,100,1,0,GetID(), intensity);
    else
      EffectVar(0, obj, effect) += intensity;

    //Blendung stark genug: Soundeffekte am Clonk
    if(intensity > 38)
    {
      if(!GetEffect("SoundDelay", obj))
      {
        AddEffect("SoundDelay", this, 1, 25, obj);
        Sound("ClonkSmallPain*.ogg", 0, obj, 100, GetOwner(obj)+1);
      }
      Sound("STUN_Bang*.ogg", false, obj, 100, GetOwner(obj)+1);
      Sound("STUN_EarRinging*.ogg", false, obj, 100, GetOwner(obj)+1);
    }
  }

  //Effekte
  Sound("STUN_Fused.ogg");
  Sparks(30,RGB(255,128));
  CastSmoke("Smoke3",12,40,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  CastParticles("PxSpark",10,60,0,0,60,120,RGBa(255,255,255,70),RGBa(255,255,255,70));
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

  //Blendwert ermitteln
  var a = BoundBy(intensity,0,255);
  //Blendobjekt erstellen, Alphawert setzen und speichern
  var flash = ScreenRGB(pTarget,RGB(255,255,255), 0, 0, false, SR4K_LayerLight);
  flash->SetAlpha(255 - a);
  if(!flash)
    return -1;
  else
    EffectVar(1,pTarget,iEffectNumber) = flash;
}

public func FxIntFlashbangTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Kein Blendobjekt vorhanden: Neues erstellen
  var flash = EffectVar(1,pTarget,iEffectNumber);
  if(!flash)
  {
    flash = ScreenRGB(pTarget,RGB(255,255,255), 0, 0, false, SR4K_LayerLight);
    EffectVar(1,pTarget,iEffectNumber) = flash;
  }

  //Aktuellen Blendwert ermitteln
  var intensity = EffectVar(0, pTarget, iEffectNumber);
  //Blendwert gleich 0: Abbruch
  if(intensity <= 0)
    return -1;
  //Zu hohen Blendwert deckeln
  if(intensity > 850)
    intensity = 850;
  //Blendwert um 2 senken
  intensity -= 2;
  EffectVar(0, pTarget, iEffectNumber) = intensity;

  //Neuen Alphawert des Blendobjekts ermitteln
  var a = flash->GetAlpha();
  a = BoundBy(intensity, 0, 255);
  //Blendobjekt aktualisieren
  flash->SetAlpha(255 - a);

  //Blendicon-Konfiguration für jeden Spieler getrennt
  var inum;
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    //Spieler, der den geblendeten Clonk steuert, auslassen
    if(i == GetOwner(pTarget))
      continue;

    //Geblendeter Clonk befindet sich in Rauch: Keine Icons setzen
    var srgb = GetScreenRGB(GetPlayerByIndex(GetOwner(pTarget)), SR4K_LayerSmoke, pTarget);
    if(srgb && srgb->GetAlpha() < 200)
    {
      Message("@", pTarget, GetPlayerByIndex(i));
      continue;
    }

    //Spieler hat keinen Clonk oder dieser nicht sein Besitz: Abbruch
    var pCursor = GetCursor(GetPlayerByIndex(i));
    if(!pCursor)
    {
      Message("@", pTarget, GetPlayerByIndex(i));
      continue;
    }

    pCursor = pCursor->~GetRealCursor();
    if(!pCursor)
	    pCursor = GetCursor(GetPlayerByIndex(i));

    //Clonk des Spielers verschachtelt und kein Pilot: Abbruch
    if(Contained(pCursor) && !(Contained(pCursor)->~GetPilot() == pCursor))
    {
      Message("@", pTarget, GetPlayerByIndex(i));
      continue;
    }

    //Clonk des Spielers ist durch Blendgranate geblendet: Abbruch
    var srgb = GetScreenRGB(GetPlayerByIndex(i), SR4K_LayerLight, pCursor);
    if(srgb && srgb->GetAlpha() < 40)
      continue;

    //Clonk des Spielers ist durch Rauch geblendet: Abbruch
    srgb = GetScreenRGB(GetPlayerByIndex(i), SR4K_LayerSmoke, pCursor);
    if(srgb && srgb->GetAlpha() < 200)
    {
      Message("@", pTarget, GetPlayerByIndex(i));
      continue;
    }

    //Bei mehreren Icons vorhergehende nicht entfernen
    var flag = 0;
    if(inum != 0)
      flag = MSG_Multiple;

    //Ansonsten Icon setzen
    if(!Contained(pTarget))
    {
      //Geblendeter im Freien: Icon gibt Blendungsgrad wieder
      if(intensity > 220)
        CustomMessage(Format("<c %x>{{SM28}}</c>", RGBa(255,255,255,BoundBy(a, 1, 254))), pTarget, GetPlayerByIndex(i), 0, 0, 0, 0, 0, flag);
      else
        CustomMessage(Format("<c %x>{{SM07}}</c>", RGBa(255,255,255,BoundBy(a, 1, 254))), pTarget, GetPlayerByIndex(i), 0, 0, 0, 0, 0, flag);
    }
    else
      //Ansonsten nur allgemeines Icon anzeigen
      CustomMessage("{{SM07}}", pTarget, GetPlayerByIndex(i), 0, 0, 0, 0, 0, flag);
    inum++;
  }
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