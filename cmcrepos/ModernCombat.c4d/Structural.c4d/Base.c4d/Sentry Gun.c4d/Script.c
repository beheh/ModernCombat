/*-- Sentry Gun --*/

#strict

local cur_Attachment;
local aim_angle;
local target_angle;
local Shooting;
local Damaged;
local iPat_Dir;
local Team;
local Active;
local Repairing;//Repariert gerade. Damit Active nicht durcheinander kommt.
local autorepair;

/* Aktionen */
public func StartRepair()
{
  if(GetAction() ne "RepairStart")
    SetAction("RepairStart");
  Repairing = true;
  RemoveEffect("ShowWeapon",this());
}

public func Repair()
{ 
  DoDamage(-GetDamage());//Es hat ja jetzt einen Schutz.
  AddEffect("IntRepair",this(),50,5,this());
}

public func FxIntRepairStart(object pTarget, int iEffectNumber, int iTemp)
{
  Sound("Weld",false,this(),50,0,+1); 
  return(1);
}

public func FxIntRepairTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(iEffectTime >= 35*20)
    return(-1);
    
  if(!Random(2))
    Sparks(2+Random(5), RGB(187, 214, 224), RandomX(-GetDefWidth()/2,+GetDefWidth()/2), RandomX(-GetDefHeight()/2,+GetDefHeight()/2));
    
  return(0);
}

public func FxIntRepairStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  Sound("Weld",false,this(),0,0,-1); 
  if(!iReason)
    pTarget->SetAction("RepairStop");
  return(0);
}

public func StopRepair()
{
  Repairing = false;
  Damaged = 0;
  DoDamage(-GetDamage());//Entschädigen.
  AddEffect("ShowWeapon",this(),1,1,this(),GetID());
  SetGraphics(0,this(),GetID(),3,5,0,0,this());
}

public func Destroyed()
{
  SetAction("Destroyed");
  Damaged = 1;
  RemoveEffect("ShowWeapon",this()); 
  AutoRepair();
  GetAttWeapon()->StopAutoFire();
}

//Startet die Reparatur automatisch wenn kaputt.
public func AutoRepair()
{
  if(autorepair)
    ScheduleCall(this(),"StartRepair",autorepair+RandomX(-50,+50));
}

public func SetAutoRepair(int iAuto)
{
  autorepair = iAuto;
}


/* Aufrufe */

public func TurnOn()
{
  Active = true;
}

public func TurnOff()
{
  Active = false;
}

public func SetTeam(int i) //Einem Team zuordnen
{
  Team = i;
}

public func Arm(id idWeapon)
{
  // Crash-Vermeidung
  if(!idWeapon) return();
  if(!GetName(0, idWeapon)) return();

  //Ausrüsten mit idWeapon
  var pWeapon = CreateObject(idWeapon, 0, 0, GetOwner());
  Enter(this(),pWeapon);
  //Ordnern
  SetObjectOrder(this(), pWeapon, 1);
  aim_angle = 180;
  iPat_Dir = -1+ Random(2)*2;
  cur_Attachment = pWeapon;
  LocalN("controller", pWeapon) = this();
  Reload();
  GetAttWeapon()->StopAutoFire();
}

/* Script */

public func Initialize() 
{
  //Ausrichten
  FitToTop();
  AddEffect("ShowWeapon",this(),1,1,this(),GetID());
  //lieber Overlay 3 nehmen. Falls wir mal 2-overlayige waffen einbaun :]
  SetGraphics(0,this(),GetID(),3,5,0,0,this());
}

public func WeaponAt(&x, &y, &r) {
  x = 0;
  y = 7000;
  r = aim_angle+270;
  return(1);
}

public func WeaponBegin(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  x = EffectVar(2, this(), number)/1000;
  y = EffectVar(3, this(), number)/1000;
}

public func WeaponEnd(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  x = EffectVar(4, this(), number)/1000;
  y = EffectVar(5, this(), number)/1000;
}

public func GetWeaponR() {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  return(EffectVar(1, this(), number));
}

public func FitToTop()
{
  //Das Gestell passt sich selbst mal etwas besser zur Decke an.
  var x = GetX(), y = GetY() - GetObjHeight() / 2;
  
  while(! GBackSolid(0, -8) )
  {
    y -= 1;
    
    //Endlosschleife vermeiden
    if(y < 0) return(1);
    
    SetPosition(x, y);
  }
  return(1);
}

public func Activity()
{
  var iHeight, iWidth, iAngle;
  //Wuah, haben wir eine Waffe?
  if(! GetAttWeapon()) return();
  //Sind wir im Eimer?
  if( Damaged) return();
  //Sind wir aktiv?
  if(! Active) return();
  if(Repairing) return();
  //Wenn nicht schon gesetzt: Turn-Action
  if(GetAction() ne "Turn")
    SetAction("Turn");

  /*Patroullie fahren*/
  
  //Zu weit links?
  if( aim_angle <= MaxRotLeft() )
  {
    //Wir fahren zurück
    iPat_Dir = 1;
  }
  //Oder zu weit rechts?
  else if( aim_angle >= MaxRotRight() )
  {
    //Hinfahren
    iPat_Dir = -1;
  }
  //Das Fahren selber ;)
  if(!Shooting || Abs(aim_angle-target_angle) > 10)
    aim_angle += iPat_Dir*5;
  else {
    if(aim_angle > target_angle)
      iPat_Dir = -1;
    else if(aim_angle < target_angle)
      iPat_Dir = 1;
    aim_angle = target_angle;
  }
       
  /* Feinde suchen */
  
  //Berechnung
  iAngle = aim_angle;
  iWidth =  Sin(iAngle, SearchLength());
  iHeight = -Cos(iAngle, SearchLength());

  if(iWidth < 0)
    Search(iWidth, -iWidth, iHeight);
  else
    Search(0, iWidth, iHeight);
  
}

public func Search(int iX, int iWidth, int iHeight)
{
  var pAim;
  
  /*DrawParticleLine("PSpark", 0, 0, -70 + Sin(aim_angle, SearchLength()), SearchLength(), 10, 80, RGB(255, 0, 0));
  DrawParticleLine("PSpark", 0, 0, 70 + Sin(aim_angle, SearchLength()), SearchLength(), 10, 80, RGB(255, 0, 0));*/ 
  
  while( pAim = FindObject(0, iX, 17, iWidth, iHeight, OCF_Alive(), 0, 0, NoContainer(), pAim ))   
  {
    // Freund-Checks
    if(Team)
      if(GetPlayerTeam(pAim->GetOwner()) == Team)
        continue;
    if(GetOwner() != NO_OWNER)
      if(pAim->GetOwner() == GetOwner() || !Hostile(pAim->GetOwner(), GetOwner()))
        continue;
    if(pAim->~NoSentryGunTarget())
      continue;
    //Weg frei?
    if(! PathFree(GetX(), GetY(), pAim->GetX(), pAim->GetY()) )
        continue;
    //Winkel zum Ziel
    target_angle = Angle(GetX(), GetY() + 7, pAim->GetX(), pAim->GetY());
    //Pseudo Suchkegel
    var iAngleX_1, iAngleX_2, iAngleY, cur_Dist; //Pseudopunkte entfernt von der Sentry
    cur_Dist = Sin(aim_angle, SearchLength());
    iAngleY = GetY() + iHeight;
    iAngleX_1 = GetX() - 70 + cur_Dist;
    iAngleX_2 = GetX() + 70 + cur_Dist;
    //Nun schauen, ob target Angle innerhalb von den Winkeln dieser Punkte ist
    if(target_angle < Angle(GetX(), GetY() + 7, iAngleX_2, iAngleY) || target_angle > Angle(GetX(), GetY() + 7, iAngleX_1, iAngleY) )
        continue;
    Shooting = true;
    if(!IsFiring())
      GetAttWeapon()->ControlThrow(this());
    if(GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage))
      Reload();
    break;
  }
  
  if( Shooting && ! pAim)
  {
    GetAttWeapon()->StopAutoFire();
    Shooting = false;
  }
}

public func IsFiring()
{
  if(!GetAttWeapon()) return(false);
  
  if(GetEffect("Recharge",GetAttWeapon()))
    return(true);
  return(false);
}

private func Reload()
{
  // Munitionsart
  var AmmoID = GetAttWeapon()->~GetFMData(FM_AmmoID);
  // Erzeugen
  Local(0, CreateContents(AmmoID)) = GetAttWeapon()->~GetFMData(FM_AmmoLoad);
  // Waffe soll nachladen
  GetAttWeapon()->~Reloaded(this());
  GetAttWeapon()->~Recharge();
}

public func Damage()
{
  if(GetDamage() > 400)
  {
    Explode(30);//Jetzt ist sie wirklich weg. :P
    return();
  }
  
  if(GetDamage() > 200)
  {
    if((GetAction() eq "Turn") || (GetAction() eq "Idle"))
    {
      Destroyed();
      CreateObject(ROCK)->Explode(20);
    }
  }
}

public func EMPShock()
{
    if(Damaged) return(0);
    //EMP Granate!1 :O
    Damaged = 1;
    AddEffect("EMP_Damaged", this(), 180, 50, this(), GetID() );
    return(1);
}

public func FxEMP_DamagedTimer( pTarget, iEffectNumber)
{    
    if( GetAttWeapon())
    {
        CreateParticle("PxSpark", RandomX(-2, 2), RandomX(10, 20), RandomX(-2, 2), 4, 30, RGB(187, 214, 224));
    }
    
    //Nach gewisser Zeit wieder anstellen.
    if( GetEffect("EMP_Damaged", this(), 0, 6) > 650)
      return(-1);
}

public func FxEMP_DamagedStop( pTarget, iEffectNumber)
{
    Damaged = false;
    Activity(1);
}

/* Konsolensteuerung */

public func ConsoleControl(int i)
{
    if(i == 1)
    {
      if(Active) return("$TurnOff$");
      else
          return("$TurnOn$");
    }
    if(i == 2)
      if(GetAction() eq "Destroyed")
        return("$Repair$");
}

public func ConsoleControlled(int i)
{
    if(i == 1)
    {
        if(Active) Active = 0;
        else
        {
            Active = 1;
        }
    }
    if(i == 2)
    {
      if(GetAction() eq "Destroyed")
        StartRepair();
    }
}

public func UpdateCharge() { return(1); }

//Für das Abfragen
public func GetAttWeapon() { return(cur_Attachment); } //Waffe
public func MaxRotLeft()   { return(110); } //Maximaler Winkel links
public func MaxRotRight()  { return(250); } //Maximaler Winkel rechts
public func SearchLength() { return(250); } //Suchlänge
public func AimAngle()     { return(aim_angle); } //Winkel auf Ziel
public func ReadyToFire()  { return(1); } //Allzeit bereit
public func IsMachine()    { return(true); } //Ist eine Elektrische Anlage
public func IsBulletTarget() { return(true); } //Kugelziel
public func IsAiming()     { return(true); } // Die Sentry Gun "zielt" immer
