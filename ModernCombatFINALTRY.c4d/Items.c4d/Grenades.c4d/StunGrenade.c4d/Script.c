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

    //Intensität errechnen
    var intensity = ((400-ObjectDistance(this,obj))*470/250)/2;

	if(intensity <= 0)
	  continue;
	
    //Ziel ein Clonk?
    if(obj->~IsClonk())
    {
      //Freie Sicht zum Clonk?
      if(!PathFree(GetX(),GetY(),GetX(obj),GetY(obj)-8))
        continue;

      //Clonk freundlich: Intensität halbieren
      if(!Hostile(GetOwner(obj), GetOwner(GetUser())))
        intensity = intensity/2;
    }
    else
    {
      if(!PathFree(GetX(),GetY(),GetX(obj),GetY(obj))) continue;
    }

    //Vorhandenen Blendeffekt des Ziels aktualisieren oder neuen anhängen
    var effect = GetEffect("IntFlashbang", obj);
    if(!effect)
      AddEffect("IntFlashbang",obj,1,1,0,GetID(), intensity, GetOwner(GetUser()));
    else
      EffectVar(0, obj, effect) += intensity;
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
  if(iTemp)
    return;

  //Keine Intensität?
  if(!intensity)
    return -1;

  EffectVar(0,pTarget,iEffectNumber) = intensity;

  //Blendung stark genug: Dann auch hörbar
  if(intensity > 38)
    Sound("STUN_Bang.ogg", false, pTarget, 100, GetOwner(pTarget)+1);

  //Stärke berechnen und Blendung erstellen
  var a = BoundBy(intensity,0,255);

  var flash = ScreenRGB(pTarget,RGB(255,255,255), 0, 0, false, SR4K_LayerLight);
  flash->SetAlpha(255-a);
  if(!flash)
    return -1;
  else
    EffectVar(1,pTarget,iEffectNumber) = flash;
}

public func FxIntFlashbangTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var rgb = EffectVar(1,pTarget,iEffectNumber);
  if(!rgb)
    return -1;

  var i = EffectVar(0, pTarget, iEffectNumber);

  //Keine Blendung mehr: Abbruch
  if(i <= 0)
    return -1;

  //"Überblendung" verhindern
  if(i > 750)
    i = 750;

  //Blendung senken
  i--;
  EffectVar(0, pTarget, iEffectNumber) = i;
/*
  var a = 255 - BoundBy(i,0,255);
  //haxxiger Taschenlampencheck
  var eff = GetEffect("FlashlightBlindness", pTarget);
  if(!eff || !((EffectVar(1, pTarget, eff) <= 0) && (a < Flashlight_MinAlpha || a > Flashlight_MaxAlpha)))
  {
    //dies hier wird nur ausgeführt, wenn entweder kein Taschenlampeneffekt vorhanden ist
    //oder der Alphawert zu klein/zu groß für die Taschenlampe ist
	rgb->SetAlpha(a);
  }
  else
    if(eff)
      return;
	else
	  a = rgb->GetAlpha();
	*/
  var a = 255 - BoundBy(i, 0, 255);
  rgb->SetAlpha(a);
  //Blendungsicon anhängen
  if(!Contained())
    for(var i = 0; i < GetPlayerCount(); i++)
    {
	  if(i == GetOwner(pTarget))
	    continue;
		
      var pCursor = GetCursor(GetPlayerByIndex(i))->~GetRealCursor();
      if(!pCursor && !(pCursor = GetCursor(GetPlayerByIndex(i))))
        continue;

      if(Contained(pCursor))
        continue;

      var srgb = GetScreenRGB(GetPlayerByIndex(i), SR4K_LayerLight, pCursor);

      if(srgb && srgb->GetAlpha() < 50) 
	  {
        CustomMessage("@", pTarget, GetPlayerByIndex(i));
		continue;
	  }
	  
	  srgb = GetScreenRGB(GetPlayerByIndex(i), SR4K_LayerSmoke, pCursor);
      
      if(srgb && srgb->GetAlpha() < 200) 
	  {
        CustomMessage("@", pTarget, GetPlayerByIndex(i));
		continue;
	  }
	  
      CustomMessage(Format("<c %x>{{SM07}}</c>", RGBa(255,255,255,BoundBy(255 - a, 1, 254))), pTarget, GetPlayerByIndex(i), 0, 0, 0, 0, 0, MSG_Multiple); 
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