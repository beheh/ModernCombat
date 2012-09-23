/*-- Sprengfallen-Appendto --*/

#strict 2
#appendto BBTP

public func Throw()
{
  //Winkel übernehmen
  var user = Contained(Contained());
  iDir = user->AimAngle();

  var x, y,			//X-/Y-Austrittsposition
  doplace,			//1: Normales Aufstellen, 2: Nach Winkel drehen
  vtx, vty;			//Offset für Zusatzvertex

  //Kriechen
  if (user->~IsCrawling())
  {
    x = 10;
    if (!GetDir(user))
      x = -10;
    y = 1;
    doplace = 1;
  }

  //Klettern
  else if (GetProcedure(user) == "SCALE")
  {
    x = 3;
    iDir = -90;
    if (!GetDir(user))
    {
      x = -3;
      iDir = 90;
    }
    vtx = x * 4 / 3;
    doplace = 2;
  }

  //Hangeln
  else if (GetProcedure(user) == "HANGLE")
  {
    y = -6;
    iDir = 160;
    if (!GetDir(user))
      iDir = -160;
    doplace = 2;
    vty = -2;
  }

  //Klettern an Leitern
  else if (GetAction(user) == "ScaleLadder")
    doplace = 1;

  //Laufen und Zielen
  else if (GetProcedure(user) == "WALK" || user->~IsAiming())
  {
    doplace = 1;
    y = 6;
  }

  //Sonst: Bereit zu feuern?
  else if (user->~ReadyToFire())
    doplace = 1;

  //Bei nicht identifizierbarer Aktion
  if (!doplace) return;
  Exit(this, x, y - GetDefOffset(GetID(), 1));

  //Effekte
  Sound("BBTP_Activate.ogg", 0, 0, 50);
  CreateParticle("PSpark", 0, 0, 0, 0, 60, GetPlrColorDw(GetOwner()), this);

  //Aktivierung
  bActive = true;
  ScheduleCall(0, "FinFuse", 2 * 36);

  //Grafik setzen
  SetAction("Fused");
  if (doplace == 1)
  {
    if(Inside(iDir, -180, -130)) SetPhase(3);
    if(Inside(iDir, -129, -78))  SetPhase(0);
    if(Inside(iDir, -77 , -27))  SetPhase(1);
    if(Inside(iDir, -26 , 25))   SetPhase(2);
    if(Inside(iDir, 26  , 76))   SetPhase(3);
    if(Inside(iDir, 77  , 128))  SetPhase(0);
    if(Inside(iDir, 129 , 179))  SetPhase(1);
    SetObjDrawTransform(1000, 0, 0, 0, 1000, 0);
  }

  if (doplace == 2)
  {
    SetPhase();
    var s = Sin(-iDir, 1000), c = Cos(-iDir, 1000);
    SetObjDrawTransform(c, s, 0, -s, c);
  }

  //Vertex zur besseren Haftung
  if (vtx || vty)
    AddVertex(vtx, vty);

  var nextmine = user->~GrabGrenade(GetID());
  if(user->~IsAiming())
    if(!nextmine)
      user->StopAiming();
}

public func ControlUp(object pObjBy)
{
  if(Contained()) return;

  if(Hostile(GetOwner(),GetOwner(pObjBy)))
    //Punkte bei Belohnungssystem (Entschärfung)
    DoPlayerPoints(BonusPoints("TechnicalTask"), RWDS_TeamPoints, GetOwner(pObjBy), pObjBy, IC15);

  //Träger = Besitzer
  controller = GetOwner(pObjBy);
  SetController(controller);
  SetOwner(controller);

  //Deaktivieren
  bActive=false;

  //Eventuell sichern
  if(bReady)
    Defuse();

	var pPack = CreateObject(BTPK, 0, 0, GetOwner(pObjBy));
	pPack->SetPackPoints(1);

  //Einsammeln wenn möglich, sonst zumindest loslassen
  if (!Collect(pPack, pObjBy))
    SetCommand(pObjBy, "UnGrab");
  
  //Claymore auf jeden Fall entfernen
  RemoveObject(this);
  	
  return true;
}
