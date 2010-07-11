/* Helicopter */

#strict 2


/* ----- Variablen ----- */

local throttle,                 //int    - Schub
      rotation,                 //int    - erwartete Drehung
      idle,                     //int    - Motor-Start/Stop Sequenz
      soundsystem,              //object - für die soundsteuerung
      view_mode;                //bool   - für den Piloten

local hud;                      //object - Anzeige für Pilot

local Pilot,                    //object - Pilot
      Gunner,                   //object - Schütze
      Rocketeer,                //object - Raketenschütze
      Passenger1, Passenger2;   //object - verschiedene Sitzplätze im Heli

local MGStation,                //object - Das MG-Objekt
      RocketStation;            //object - Das Raketenwerfer-Objekt

local s_counter,                //int    - eine kleine Counter-Variable für Warnsounds
      d_counter;                //int    - eine kleine Counter-Variable für die Zerstörung

local destroyed;                //bool   - ob der Heli schon zerstört ist

local fuel;

static const throttle_speed = 5;//int    - "Feinfühligkeit"
static const rot_speed = 1;     //int    - Drehgeschwindigkeit / frame
static const control_speed = 3; //int    - "Feinfühligkeit"
static const max_throttle = 200;//int    - höchste Schubeinstellung
static const max_rotation = 30; //int    - stärkste erlaubte Neigung

/* ----- Callbacks ----- */

public func IsMachine() { return 1; }
public func MaxDamage() { return 300; }
public func IsBulletTarget() { return 1; }

public func Passengers()
{
  var arr = [];
  if(Pilot)
    arr[GetLength(arr)] = Pilot;
  if(Gunner)
    arr[GetLength(arr)] = Gunner;
  if(Rocketeer)
    arr[GetLength(arr)] = Rocketeer;
  if(Passenger1)
    arr[GetLength(arr)] = Passenger1;
  if(Passenger2)
    arr[GetLength(arr)] = Passenger2;
  
  return arr;
}


/* ----- Existenz ----- */

//Initialisierung
protected func Initialize()
{
  //Steuerung initialisieren
  throttle = 0;
  rotation = 0; 
  //Stuff.
  fuel = 999999; //Blar?
  SetAction("Stand");

  //Pilot
  view_mode = true;
  
  //Geschütze
  MGStation = CreateObject(H_MA,0,0,GetOwner());
  MGStation -> Set(this);
  //RocketStation = CreateObject(HRKS,0,0,GetOwner());
  //RocketStation -> Set(this);

  //Eingang
  SetEntrance(true);
  //fertig
  return true;
}

//wird genutzt, den Landport davon abzuhalten, den zerstörten Heli zu versorgen
func IsReady()
{
  return GetDamage() < MaxDamage() && GetContact(this(), -1, CNAT_Bottom) && !destroyed;
}

//alle dazugehörigen Objekte löschen
protected func Destruction()
{
  if(hud)
    RemoveObject(hud);
  if(MGStation)
    RemoveObject(MGStation,true);
  if(RocketStation)
    RemoveObject(RocketStation,true);
  return true;
}

/* ----- Eingangssteuerung ----- */

//herausgeworfene Objekte sollen vor der Tür erscheinen
protected func ControlCommand(string Command, object Target, int TargetX, int TargetY, object target2, int Data, object ByObj)
{
  if (Command == "Exit")
  {
    var rot = GetDir()*180-90 + GetR() + GetDir()*120-60;
    Exit(ByObj, Sin(rot,25), -Cos(rot,25), GetR(), GetXDir(0,1), GetYDir(0,1), GetRDir());
    return true;
  }
  return false;
}

protected func Collection2(object pObj)
{
  if(GetOCF(pObj) & OCF_Alive && GetOCF(pObj) & OCF_CrewMember)
  {
    if(!Hostile(GetOwner(this),GetOwner(pObj)))
    {
      if(!Pilot)
        return EnterSeat1(0,pObj); //Blöde Platzhalter :/
      if(!Gunner)
        return EnterSeat2(0,pObj);
      if(!Rocketeer)
        return EnterSeat3(0,pObj);        
      if(!Passenger1)
        return EnterSeat4(0,pObj);        
      if(!Passenger2)
        return EnterSeat5(0,pObj);
    }
    else //Feindliche Clonks kommen nicht rein.
      Exit(pObj);
  }
}
                
/* ----- Steuerung ----- */

//Nur für Pilot: Mehr Schub
protected func ContainedUp(object ByObj)
{
  //alle anderen außer dem Pilot
  if (ByObj != Pilot) return true;
  //Startup-Sequence
  if (throttle == 0 && (GetAction()=="Stand" || GetAction()=="EngineShutDown"))
    SetAction("EngineStartUp");
  //beim Flug mehr Schub
  if (GetAction()=="Fly" || GetAction()=="Turn")
    throttle = BoundBy(throttle + throttle_speed, 0, max_throttle);
  return true;
}

//Nur für Pilot: Weniger Schub
protected func ContainedDown(object ByObj)
{
  //alle anderen außer dem Piloten
  if (ByObj != Pilot)
    return true;
  //Motor aus
  if(throttle == 0 && (GetAction()=="Fly" || GetAction()=="EngineStartUp"))
    SetAction("EngineShutDown");
  //vom Gas weg
  if(GetAction()=="Fly" || GetAction()=="Turn")
    throttle = BoundBy(throttle - throttle_speed, 0, max_throttle);
  //aber nicht während dem Flug
  return true;
}

//Nur für Pilot: Mehr Schub
protected func ContainedUpDouble(object ByObj)
{
  [$CtrlUp$]
  //alle anderen außer dem Piloten
  if(ByObj != Pilot)
    return true;

  if(throttle == 0 && (GetAction()=="Stand" || GetAction()=="EngineShutDown"))
    SetAction("EngineStartUp");  
  if(GetAction()=="Fly")
    throttle = BoundBy(throttle + throttle_speed*2, 0, 170);
  return true;
}

//Nur für Pilot: Weniger Schub
protected func ContainedDownDouble(object ByObj)
{
  [$CtrlDown$]
  //alle anderen außer dem Piloten
  if (ByObj != Pilot) return true;

  //Motor aus
  if (throttle == 0 && (GetAction()=="Fly" || GetAction()=="EngineStartUp")) SetAction("EngineShutDown");
  //vom Gas weg
  if (GetAction()=="Fly") throttle = BoundBy(throttle - throttle_speed*2, 0, 170);
  //aber nicht während dem Flug
  return true;
}

//Nur für Pilot: Nach links neigen
protected func ContainedLeft(object ByObj)
{
  //alle anderen außer dem Piloten
  if (ByObj != Pilot) return true;

  if (GetAction()=="Fly" || GetAction()=="Turn") 
    rotation = BoundBy(rotation - control_speed, -max_rotation, max_rotation);
  return true;
}

//Nur für Pilot: Nach rechts neigen
protected func ContainedRight(object ByObj)
{
  //alle anderen außer dem Piloten
  if (ByObj != Pilot) return true;
     
  if (GetAction()=="Fly" || GetAction()=="Turn")
    rotation = BoundBy(rotation + control_speed, -max_rotation, max_rotation);
  return true;
}

//Nur für Pilot: Nach links umdrehen
protected func ContainedLeftDouble(object ByObj)
{
  [$CtrlLeftD$]
  //alle anderen außer dem Piloten
  if (ByObj != Pilot) return true;

  if (GetDir() && GetAction()=="Fly")
  {
    if (GetAction() == "Turn" || GetContact(this(), -1)) return true;
    SetAction("Turn");
  }
  return true;
}

//Nur für Pilot: Nach rechts umdrehen
protected func ContainedRightDouble(object ByObj)
{
  [$CtrlRightD$]
  //alle anderen außer dem Piloten
  if (ByObj != Pilot) return true;

  if (!GetDir() && GetAction()=="Fly")
  {
    if (GetAction() == "Turn" || GetContact(this(), -1)) return true;
    SetAction("Turn");
  }
  return true;
}

//eine Funktion, welche einfach nur die Richtung eines Objektes ändert
protected func ChangeDir()
{
  SetDir(!GetDir());
  SetVertex(0, false, 15*(GetDir()*2-1), this(), 2);
  return(true);
}

//HUD an-/ausschalten
protected func ContainedThrow(object ByObj)
{
  [Image=KOKR|$CtrlThrow$]
  //nicht wenn kaputt
  if (GetDamage() > MaxDamage()) return(true);
  //der Pilot kriegt eins...
  if (ByObj == Pilot)
  {
    if (!hud)
    {
      hud = CreateObject(H_H0, 0, 0, GetOwner());
      LocalN("heli", hud) = this();
    }
    else
      RemoveObject(hud);
    return(Sound("SwitchHUD", false, this(), 100, GetOwner(ByObj)+1));
  }
  //Alle anderen sind traurig :(
  return(false);
}


/* ----- Platzwechsel ----- */

//Platz wechseln
protected func ContainedDigDouble(object ByObj)
{
  [$CtrlDigD$]
  CreateMenu(GetID(),ByObj,this(),0,"$Seats$",0,1);
    //Ausstieg
    //Pilot
    if(Pilot)
      AddMenuItem("<c ff8888>Pilot</c>", "SeatOccupied()",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>Pilot</c>", "EnterSeat1",GetID(),ByObj,0,ByObj,"$PilotSeat$");
    //MG-Schütze
    if(Gunner)
      AddMenuItem("<c ff8888>$Gunner$</c>", "SeatOccupied()",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Gunner$</c>", "EnterSeat2",GetID(),ByObj,0,ByObj,"$GunnerSeat$");
    //Raketen-Schütze
    if(Rocketeer)
      AddMenuItem("<c ff8888>$Rocketeer$</c>", "SeatOccupied()",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Rocketeer$</c>", "EnterSeat3",GetID(),ByObj,0,ByObj,"$RocketeerSeat$");
    //Passagier 1
    if(Passenger1)
      AddMenuItem("<c ff8888>$Passenger1$</c>", "SeatOccupied()",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Passenger1$</c>", "EnterSeat4",GetID(),ByObj,0,ByObj,"$PassengerSeat$");
    //Passagier 2
    if(Passenger2)
      AddMenuItem("<c ff8888>$Passenger2$</c>", "SeatOccupied()",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Passenger2$</c>", "EnterSeat5",GetID(),ByObj,0,ByObj,"$PassengerSeat$");
      
  return 1;
}

//Sitz besetzt?
private func SeatOccupied(a,ByObj)
{
  Log("%v %v",a,ByObj);
  PlayerMessage(GetOwner(ByObj),"$SeatOccupied$",ByObj);
  Sound("Error",0,0,0,GetOwner(ByObj)+1);
  ContainedDigDouble(ByObj);
  return 1;
}

//Sitz reinigen *hrhr*
private func DeleteActualSeatPassenger(object Obj)
{
  if(Pilot == Obj)
  {
    Pilot = 0;
    if(hud)
      RemoveObject(hud);
  }
  if(Gunner == Obj)
    Gunner = 0;
  if(Rocketeer == Obj)
    Rocketeer = 0;
  if(Passenger1 == Obj)
    Passenger1 = 0;
  if(Passenger2 == Obj)
    Passenger2 = 0;
  return 1;
}

//Sitz belegen
public func EnterSeat1(a, object Obj)
{
  Log("%v %v",a,Obj);
  DeleteActualSeatPassenger(Obj);
  Pilot = Obj;
  Sound("SwitchHUD", false, this(), 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat2(a, object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Gunner = Obj;
  Enter(MGStation,Obj);
  Sound("SwitchHUD", false, this(), 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat3(a, object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Rocketeer = Obj;
  Sound("SwitchHUD", false, this(), 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat4(a, object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Passenger1 = Obj;
  Sound("SwitchHUD", false, this(), 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat5(object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Passenger2 = Obj;
  Sound("SwitchHUD", false, this(), 100, GetOwner(Obj)+1);

  return 1;
}


/* ----- Schaden ----- */

public func OnDmg(int iDmg, int iType) {
	
	if(iType == DMG_Energy)		return(-20);
	if(iType == DMG_Bio)		return(100);
	if(iType == DMG_Melee)		return(80);
	if(iType == DMG_Fire)		return(50);
	if(iType == DMG_Explosion)	return(-30);
	if(iType == DMG_Projectile)	return(70);
	
	return(50);
}

public func Damage()
{
	if(GetDamage() < MaxDamage()) return;

	DestroyHeli();
}

//hier wird der Heli effektreich zerstört
func DestroyHeli()
{
  //alles raus
  for(var obj in FindObjects(Find_Container(this)))
    Exit(obj, 0, 0, Random(360), RandomX(-10,10), RandomX(-5,12), Random(10));
    
  Explode(60);
  Sound("MainHelicopterExplosion", false, this());

  //zum Wrack machen
  var obj;
  obj = CreateObject(H_HW, 0, 20, -1);
  obj -> Incinerate();
  obj -> SetR(GetR());
  obj -> SetXDir(GetXDir());
  obj -> SetYDir(GetYDir());
  obj -> SetRDir(GetRDir());
  for (var i; i < 7; i++)
  {
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255)); 
  }
  return(RemoveObject());
}

//Kontakt am Rotor: Schaden!
protected func ContactTop()
{
  if (!(GetCon() == 100)) return(false);
  //sehr viel schaden
  DoDamage(25, this());
  for (var i; i < GetVertexNum(); i++)
  {
    if (GetContact(0, i))
      CreateParticle("Blast", GetVertex(i), GetVertex(i, true),
                     0, 0, 50, RGB(255,255,255));
  }
  Sound("Collision*");
}

//die Räder halten mehr aus
protected func ContactBottom()
{
  if (GetYDir() > 25)
  {
    for (var i; i < GetVertexNum(); i++)
    {
      if (GetContact(0, i))
        CreateParticle("Blast", GetVertex(i), GetVertex(i, true),
                       0, 0, 50, RGB(255,255,255));
    }
    DoDamage(GetYDir()/2);
    Sound("Collision*");
  }
}

//normaler Rumpf, auch recht, habe mir aber einen Callback gespart
protected func ContactLeft()
{
  if (Abs(GetXDir()) > 20 || Abs(GetYDir()) > 20)
  {
    for (var i; i < GetVertexNum(); i++)
    {
      if (GetContact(0, i))
        CreateParticle("Blast", GetVertex(i), GetVertex(i, true),
                       0, 0, 50, RGB(255,255,255));
    }
    DoDamage(Abs(GetXDir())+Abs(GetYDir()));
    Sound("Collision*");
  }
}

//Objekt, die in den Rotor geraten, verursachen Schaden
protected func RejectCollect(id ID, object ByObj)
{
  //man soll schon Objekte im Heli ablegen können
  if (Contained(ByObj))
    return(false);
  //von außen heißt es: Schaden!!!
  if (GetAction()!="Stand")
  {
    var dir = (!GetDir())*2-1;
    DoDamage(GetMass(ByObj));
    ProtectedCall(ByObj, "Hit");
    SetXDir((Random(30)+30)*dir, ByObj);
    SetYDir(   RandomX(-20,-20), ByObj);
    return(true);
  }
  else if (GetOCF(ByObj) & OCF_HitSpeed2)
  {
    ProtectedCall(ByObj, "Hit");
    return(true);
  }
  return(true);
}
                           
//für Warnsounds und Grafik zuständig
protected func TimerCall()
{
  //unter Wasser stirbt der Motor ab
  Water();

  //Piloten anpassen
  DrawPilot();

  //bis 50% nichts
  if (GetDamage() < MaxDamage()*1/2) return(false);

  // ab 50% rauchen
  Smoking();

  if (GetDamage() < MaxDamage()*3/4) return(false);

  // ab 25% Feuer
  DrawFire();

  if (!GetEffect("Engine", this())) return(false);
  
  //Warnsound
  WarningSound();
}

private func DrawPilot()
{
  if (view_mode && !Pilot)
  {
    view_mode = false;
    SetColorDw(RGBa(200,200,200,255));
  }
  else if (!view_mode && Pilot)
  {
    view_mode = true;
    SetColorDw(RGBa(200,200,200,0));
  }
}

private func WarningSound()
{
    if (GetDamage() < MaxDamage()*3/4) 
  {
    //Sound("DamageCritical", false, this());
    if (!(s_counter%36) && soundsystem)
    {
      var obj;
      for (var i; i < ContentsCount(0, this()); i++)
      {
        if (obj = Contents(i, this()))
          if (GetOCF(obj) & OCF_CrewMember)
           Sound("DamageWarning", false, soundsystem, 100, GetOwner(obj)+1);
      }
    }
    s_counter++;
    if (s_counter >= 100) s_counter = 0;
    
    return(false);
  }
  else
  {
    if (!(s_counter%20) && soundsystem)
    {
      var obj;
      Local(2) = 0;
      for (var i; i < ContentsCount(0, this()); i++)
      {
        if (obj = Contents(i, this()))
          if (GetOCF(obj) & OCF_CrewMember)
            Sound("DamageCritical", false, soundsystem, 100, GetOwner(obj)+1);

      }
    }
    s_counter++;
    if (s_counter >= 100) s_counter = 0;

    return(false);
  }
}

private func DrawFire()
{
  var dir = GetDir()*2-1;
  if (!GBackLiquid(-Sin(GetR()+dir*80, 25), +Cos(GetR()+dir*80, 25)))
  {
    CreateParticle("Blast",-Sin(GetR()+dir*80, 25),
                           +Cos(GetR()+dir*80, 25),
  			             0, -10, 100+Random(20), RGB(255,255,255), this());
  }
}

private func Water()
{
  if (GBackLiquid(0,10) && GetEffect("Engine", this()))
  {
    DoDamage(5);
    SetAction("EngineShutDown");
  }  
}

private func Smoking()
{
  var dir = GetDir()*2-1;

  for (var a = 0; a < 3; a++)
  { 
    if (!GBackLiquid(-Sin(GetR()+dir*80, 25), +Cos(GetR()+dir*80, 25)))
      Smoke(-Sin(GetR()+dir*80, 25), +Cos(GetR()+dir*80, 25), Random(10));
  }
}

/* ----- Physik ----- */

//Motor anlassen
protected func StartEngine()
{
  Sound("StartSystem", false, this());
}

//Motor läuft
protected func EngineStarted()
{
  Log("%v",this);
  AddEffect("Engine",this,300,1,this,0);
  soundsystem = CreateObject(H_SS, 0, 0, GetOwner());
  LocalN("heli", soundsystem) = this;
  throttle = 10;
  rotation =  0;
}

//Motor abstellen/ abgestorben
protected func StopEngine()
{
  if (!fuel) PlayerMessage(GetOwner(), "<c ff0000>$MsgNoFuel$</c>", this());
  if (soundsystem) RemoveObject(soundsystem);

  Sound("StopSystem", false, this()); 
  RemoveEffect("Engine",this());
}

//Motor aus
protected func EngineStopped()
{
  throttle = 0;
  rotation = 0;
}


/* ----- Effekt: Engine ----- */

protected func FxEngineStart(object Target, int EffectNumber, int Temp, Var1, Var2, Var3, Var4)
{
  if (Temp) return(true);
}

protected func FxEngineStop(object Target, int EffectNumber, int Reason, bool Temp)
{
  if (Temp) return(true);
}

//hier wird das Flugverhalten gesteuert
protected func FxEngineTimer(object Target, int EffectNumber, int EffectTime)
{
  //Variablen abspeichern
  var rot = LocalN("rotation", Target);
  var thr = LocalN("throttle", Target);
  var Fg, Fv, Fh, Fw, Fs, dFv, dFh, m, mh, g, av, ah;  //physikalische Kräfte
  
  //Überprüfung, ob überhaupt noch ein Pilot drin...
  var has_pilot = (LocalN("Pilot", Target));

  //Rotation anpassen
  if (has_pilot)
  {
    var speed;
    speed = BoundBy(rot-GetR(Target), -rot_speed, rot_speed);
    SetRDir(speed, Target);
  }
  else
  {
    LocalN("rotation", Target) = GetR(Target);
  }
  //Treibstoff verbrauchen
  if (!(EffectTime % 10) && !ObjectCount(H_NP))
  {
    LocalN("fuel", Target) -= thr;
    if (LocalN("fuel", Target) <= 0)
      Target -> SetAction("EngineShutDown");
  }

  //Gewichtskraft berechnen
  m = GetMass(Target);
  g = GetGravity();
  Fg = (m + mh) * g;
  Fw = GetWind(0, -20, false) * 200;
 
  //Hubkraft und vertikale Beschleunigung berechnen
  Fv  = - Cos(GetR(Target), 1500*thr);
  dFv = Fg + Fv;
  av  = dFv / m; //BoundBy(dFh / m, -80, 80);

  //Kraft nach links oder rechts sowie deren Beschleunigung berechnen
  Fh  = - Sin(-GetR(Target), 1500*thr);
  dFh = Fh + Fw + Fs;
  ah  = dFh / m; //BoundBy(dFf / m, -80, 80);

  //Geschwindigkeit anpassen
//Message("%dN %dN|%dN|a=%d", Target, Fg, Fh, dFh, av);
  SetYDir(GetYDir(Target, 95) + av, Target, 100);
  SetXDir(GetXDir(Target, 95) + ah, Target, 100);

  return(true);
}

//eine neue Funktion: Tangens = Sinus / Kosins
global func Tan(int angle, int radius)
{
  return(Sin(angle, radius, 10) / Cos(angle, radius, 10));
}
