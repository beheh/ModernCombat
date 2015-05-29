/*-- Respawnpoint --*/

#strict 2

local target,id;
local xdir,ydir,r;
local frames,distance;

public func IsSpawnpoint()	{return true;}


/* Globale Aufruffunktion */

global func AutoRespawn(int iFrames, int iDistance, object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return;

  //Position ermitteln und Spawnpoint erstellen
  var x = -(GetDefCoreVal("Offset", 0, GetID(pTarget), 0)) - GetDefWidth(GetID(pTarget)) / 2;
  pTarget->CreateObject(RSPT, x, GetDefHeight(GetID(pTarget))+GetDefCoreVal("Offset", 0, GetID(pTarget), 1), NO_OWNER)->Set(iFrames, iDistance, pTarget);

  return pTarget;
}

/* Initalisierung */

func Initialize()
{
  return 1;
}

/* Einstellung */

func Set(int iFrames, int iDistance, object pTarget)
{
  //Kein Hostobjekt definiert: Abbruch
  if(!pTarget) return RemoveObject();
  //Keine Respawnzeit definiert: Standardwert nutzen
  if(!iFrames) iFrames = 35*30;

  //Spawnpoint-Sichtbarkeit und -besitzer konfigurieren
  SetVisibility(VIS_None);
  SetGraphics(0, this, GetID(pTarget), 1, GFXOV_MODE_Base, 0, GFX_BLIT_Additive);
  SetOwner(GetOwner(pTarget));
  SetClrModulation(GetClrModulation(pTarget)); 

  //Positions-, Rotations- und Zeitwerte setzen
  xdir = GetXDir(pTarget);
  ydir = GetYDir(pTarget);
  r = GetR(pTarget);
  frames = iFrames;
  distance = iDistance;
  target = pTarget;
  id = GetID(pTarget);

  //Scan-Effekt starten
  AddEffect("IntScan", this, 10, Min(iFrames,35*3), this, GetID()); 
}

/* Scan-Effekt */

public func FxIntScanTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Respawn-Effekt vorhanden: Abbruch
  if(GetEffect("IntRespawn",this)) return;

  //Hostobjekt verloren: Respawn-Effekt starten
  if(!target) return StartRespawn();

  //Hostobjekt ein Lebewesen: Respawn-Effekt bei Tod starten
  if(GetOCF(target) & OCF_Living)
    if(!GetAlive(target))
      return StartRespawn();

  //Distance-Wert vorhanden: Respawn-Effekt starten, wenn das Hostobjekt sich entsprechend weit vom Respawnpoint entfernt hat
  if(distance)
    if(ObjectDistance(target) > distance)
      return StartRespawn();
}


/* Respawn-Effekt */

func StartRespawn()
{
  //Respawn-Effekt starten
  if(!GetEffect("IntRespawn"))
    AddEffect("IntRespawn", this, 25, frames, this, GetID()); 
}


public func FxIntRespawnTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Prüfbereich ermitteln
  var w = GetDefWidth(id); var h = GetDefHeight(id);
  var x = -w/2; var y = -h;

  //Im Prüfbereich nach Lebewesen suchen
  if(ObjectCount2(Find_InRect(x,y,w,h),Find_OCF(OCF_Alive)))
  {
    if(GetEffect(0,pTarget,iEffectNumber,3) > 35)
      //Nach 35 Frames erneut prüfen
      ChangeEffect("IntRespawn",pTarget,0,"IntRespawn",35);
    return;
  }

  //Ansonsten Respawn einleiten
  Respawn();

  return -1;
}

/* Hostobjekt erstellen */

func Respawn()
{
  //Neues Objekt erstellen und entsprechend färben, drehen, platzieren und einblenden
  var obj = CreateObject(id,0,-1,GetOwner());
  target = obj;
  SetR(r, obj);
  SetClrModulation(GetClrModulation(), obj); 
  SetXDir(xdir, obj);
  SetYDir(ydir, obj);
  obj->FadeIn4K(10);

  //Sofern das Objekt nicht über SolidMask verfügt: Freimachen wenn feststeckend
  if(!GetDefCoreVal("SolidMask",0,GetID(obj),3)) return;
  for(var o in obj->FindObjects(Find_InRect(-GetObjWidth(obj)/2,-GetObjHeight(obj),GetObjWidth(obj),GetObjHeight(obj)),
  				Find_Category(C4D_Vehicle | C4D_Living | C4D_Object),
  				Find_NoContainer(),
  				Find_Exclude(obj)))
  {
   AutoUnstuck4K(o);
  }
}