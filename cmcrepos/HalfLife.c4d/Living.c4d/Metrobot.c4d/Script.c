/*---- MetroCop ----*/

#strict
#include HZCK

/* Allgemeines */
public func IsCombine(){return(true);}

static const MCOM_SightRange = 300;

protected func Initialize()
{
  if(GetOwner() == NO_OWNER)
  {
    SetAction("Walk");
    //GiveWeapon(H2PW);
    AddEffect("IntAI",this(),200,20,this());
  }
}

global func GiveWeapon(id idItem,object pTarget)
{
  if(!pTarget) pTarget = this();
  if(!pTarget) return(0);

  var wpn = pTarget->CreateContents(idItem);//Waffe geben ...
  wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));
  
  DoAmmo(wpn->GetFMData(FM_AmmoID,1),9999,pTarget);//Höhöhöhöh!
  
  return(wpn);
}

/* Geräusche */
public func Death()
{
  var plr = GetOwner();

  // Sound und Meldung
  Sound("MCOP_Die*.ogg");
  DeathAnnounce(GetOwner(),this(),GetKiller());

  if(GetPlayerType(GetOwner()) == C4PT_Script)
    GameCallEx("RelaunchPlayer",GetOwner(),this(), GetKiller());
  else
  {
    var gotcrew;
    for(var i; i < GetCrewCount(plr); i++)
      if(GetOCF(GetCrew(plr,i)) & OCF_Alive)
        gotcrew = true;
  
    if(!gotcrew)
      GameCallEx("RelaunchPlayer",GetOwner(),this(), GetKiller());
  }

  if(ammobag)
    RemoveObject(ammobag);

  // Ausrüstung ablegen
  if(HasGear()) TakeOffGear();

  if(GetEffect(0,0,wpneffect))
    RemoveEffect(0,0,wpneffect);

  FadeOut(this());
  return(1);
}

public func Hurt()
{
  Sound("MCOP_Hurt*.ogg");
  return(1);
}

public func HurtSounds(int iDmg, int iType)
{
  iDmg = -iDmg;
  if(iDmg <= 0) return();
  
  var val = BoundBy(iDmg,1,30);
  if(Random(val)*100/val < 50) return();

  Sound("MCOP_Hurt*.ogg");
}

/* KI */
public func FxIntAITimer(object pTarget, int iEffectNumber)
{
  AI();
}

local enemy;

public func AI()
{
  if(!GetAlive()) return();
  
  CheckContents();
  CheckAvoid();
  
  if(!Combat())
    Idle();
}

private func Combat()
{
  //Haben wir einen Gegner?
  if(enemy)
  {
    if(!enemy->GetAlive() || (ObjectDistance(this(),enemy) > MCOM_SightRange))
      enemy = 0;
  }
  
  if(!enemy)
    enemy = FindEnemy();
    
  if(!enemy)
    return(false);


  //Können wir angreifen?
  if((ObjectDistance(enemy,this()) > GetAttackDistance()) || !PathFree(GetX(),GetY(),GetX(enemy),GetY(enemy)))
  {
    MoveToEnemy();
    return(true);
  }
  
  if(!Ready() || !IsArmed())
    return(true);//Er soll nicht Idle ausführen. :X
    
    
  //Angriff!
  TurnToObject(enemy);
  
  //Müssen wir zielen?
  if(CanAim() && ((Abs(GetY(enemy)-GetY()) > 10) || Contents()->GetFMData(FM_Aim)))
  {
    if(!AimOnTarget(enemy))
      return(true);
    Fire();
  }
  else
  {
    if(Abs(GetY(enemy)-GetY()) < 10)
    {
      Fire();
      if(Random(3))
        MoveToEnemy();
      else
        StopMoving();
    }
    else
    {
      StopFire();
    
      //TODO: Es sollte nur bis auf eine effektive Distanz herangegegangen werden.
      if(ObjectDistance(enemy,this()) >= GetAttackDistance())
      {
        MoveToEnemy();
      }
      else
      {
        if(Random(3))
          MoveToEnemy();
        else
          StopMoving();
      }
    }
  }
  
}

private func GetAttackDistance()
{
  var wpn = Contents();
  if(wpn)
    if(wpn->~IsWeapon())
      return(wpn->GetBotData(BOT_Range));
  return(0);
}

private func StopMoving()
{
  FinishCommand(this(),false,0);
  SetComDir(COMD_Stop);  
}

private func FindEnemy()
{
  var only_visible = false;
  var target = 0;
  var last_vis = false;
  var distance = MCOM_SightRange;
  
  var aScan;
  
  aScan = FindObjects(Find_Distance(distance), Find_OCF(OCF_Prey), Find_Not(Find_Func("IsCombine")));
  
  for(var scan in aScan)
  {
    if(!GetAlive(scan)) continue;
  
    var x = GetX();
    var y = GetY();
    //WeaponEnd(x,y);
    
    var visible = PathFree (x,y,GetX(scan),GetY(scan));
    var new_distance = ObjectDistance(this(),scan);
    
    if(visible)//Wenn sichtbar ab jetzt nur noch sichtbare Feinde testen!
      only_visible = true;
    else
      if(new_distance >= MCOM_SightRange/3)//Wenn der unsichtbare Feind weniger als die X des Suchradius entfernt ist, dann ignoriert er ihn!
        continue;
        
    if(only_visible)//Wenn only_visible aktiv ist unsichtbare Feinde blocken!
      if(!visible)
        continue;
    
    if((distance > new_distance)|(last_vis != visible))
    {
      distance = new_distance;
      target = scan;
      last_vis = visible;
    }
  }
  
  return(target);
}

private func Idle()
{
  if(!Random(120))
  {
    var aText = ["-.-","*schnarch*","...","Los! Ich will rumballern.","Kommt raus ihr Angsthasen!","O-o Nix los hier?","*gähn*"];
    ChatMessage(aText[Random(GetLength(aText))]);
  }
  else
  {
    if(!Random(20))
    {
      //Rumgucken.
      if(GetDir() == DIR_Left)
        SetDir(DIR_Right);
      else
        SetDir(DIR_Left);
    }
  }
}

private func ChatMessage(string szMessage, object pObj)
{
  if(!pObj) pObj = this();
  if(!pObj) return();
  Schedule("Message(\" \",this())",35*4,0,pObj); 
  return(Message("@<c %x>\"%s\"</c>",pObj,RGB(255,255,255),Format(szMessage,...)));
}

private func CheckContents()
{
  UseMedkits();
  UseArmor();
  UseAmmo();
  if(SelectWeapon())
    UpdateWeapon();
  
  for(var item in FindObjects(Find_Container(this())))
    item->~Think(0,this());
}

private func CheckAvoid()
{
  for(var obj in FindObjects(Find_InRect(-30,-30,60,60),Find_Func("IsDangerous4AI")))
  {
    return(Avoid(obj));
  }
  return(false);
}

private func Avoid(object pObj)
{
  //In welche Richtung muss gesprungen werden?
  var dir = +1;
  if(GetX(pObj) < GetX())
    dir = -1;
    
  if(Distance(GetXDir(pObj),GetYDir(pObj)) >= 2)
  {
    //Überspringen! :>
    if(dir == +1)
      dir = -1;
    else
      dir = +1;
  }
  
  //Bringt das Springen denn überhaupt was?
  if(SimJumpCheck(dir))
  {
    //Wegdrehen.
    if(dir == -1)
      SetDir(DIR_Right);
    else
      SetDir(DIR_Left);
      
    //Hops!
    Jump();
    return(true);
  }
  
  return(false);
}

private func SimJumpCheck(int iDir)//iDir =-1 oder +1
{
  //Bei 0 selbst ermitteln.
  if(!iDir)
  {
    iDir = +1;
    if(GetDir() == DIR_Left)
      iDir = -1;
  }

  // Sprungparameter ermitteln
  var x=GetX(), y=GetY();
  
  var vx = +GetPhysical("Walk")*iDir*7/250;
  var vy = -GetPhysical("Jump")/10;
  
  var vyo = vy;
  
  if (!SimFlight(x, y, vx, vy, 25, 1000, -1, 1000))// Bodenlos: Kein Sprung
    return(false);

  // Nicht zu tief nach unten, oder gegen Wand/Decke
  if (!Inside(vy, 0, vyo*-2))
    return(false);
  
  // Nicht in Lava oder Säure
  if(CheckBadMaterial(x,y))
    return(false);

  // Ansonsten OK
  return(true);
}

private func CheckBadMaterial(int iX, int iY)
{
  var iTargetMat = GetMaterial(iX,iY);
  if(GetMaterialVal("Incindiary", "Material", iTargetMat) || GetMaterialVal("Corrosive", "Material", iTargetMat))
    return(true);
  return(false);
}

private func SelectWeapon()
{
  if(!IsArmed())
  {
    for(var obj in FindObjects(Find_Container(this())))
    {
      if(obj->~IsWeapon())
      {
        if(HasAmmo(obj))
        {
          ShiftContents (this(),0,GetID(obj));
          return(true);
        }
      }
    }
    return(false);
  }
  else return(true);
}

private func UpdateWeapon()//TODO: Soll die KI mehrere Waffen haben oder wegwerfen?
{
  /*if(IsArmed())
  {
     Contents()->~SetUser(this()); 
     Contents()->~SetOwner(GetOwner());
  }*/
}

private func UseAmmo()
{
  //TODO: KI soll auch Munipakete benutzen können.
}

private func UseArmor()
{
  if(!HasGear(GEAR_Armor))
  {
    var harm = FindContents(HARM); 
    if(harm)  harm->Activate(this());
  }
}

private func UseMedkits()
{
  if(GetEnergy() <= 50)//Damit es sich auch lohnt.
  {
    var medi = FindContents(MEDI); 
    if(medi)  medi->Activate(this());
  }
}

private func CanAim()
{
  if(IsAiming())
    return(true);

  if(!GetEffect("SquatAimTimeout") && (GetAction() eq "WalkArmed" || GetAction() eq "Walk") && IsArmed())
    return(true);

  return(false);
}

private func TurnToObject(object pObj)
{
  if(GetX(pObj) > GetX())
    SetDir(DIR_Right());
  else
    SetDir(DIR_Left());
}

private func MoveToEnemy()
{
  if(!enemy) return();

  StandUp();
      
  var offx = 20;
  if(GetX(enemy)-GetX() > 0)
    offx = -offx;

  SetCommand(this(),"MoveTo",0,GetX(enemy)+offx,GetY(enemy),false);
}

private func Ready()
{
  if(ReadyToFire()) return(true);
  
  if(Contents())
    if(Contents()->GetFMData(FM_Aim))
      if(CanAim())
        return(true);
}

private func AimOnTarget(object pTarget)
{
  if(!pTarget) pTarget = enemy; 
  if(!pTarget) return(false);
  if(!Ready()) return(false);
  
  TurnToObject(pTarget);
    
  if(!IsAiming())
    StartSquatAiming();
  
  
  var iAngle = Angle(GetX(),GetY(),GetX(pTarget),GetY(pTarget));
  if(Abs(AngleOffset4K(iAngle,GetAiming())) > 10)
  {
    iAngle = iAngle + RandomX(-10,+10);//Ungenauigkeit
    if(iAngle > 180)
      iAngle -= 360;

    //Winkel wird zu groß?
    iAngle = BoundBy(iAngle,-AIM_Max,+AIM_Max);
    
    SetAiming(iAngle);
  }
    
  return(true);
}

private func GetAiming()
{
  if(!crosshair)
  {
    if(GetDir() == DIR_Left)
      return(180);
    else
      return(0);
  }
  
  return(crosshair->GetAngle());
}

private func SetAiming(int iAngle)
{
  if(!crosshair)//Notfallhack
  {
    StopAiming();
    StartSquatAiming();
  }
  
  if(!Inside(crosshair->GetAngle()-iAngle,+20,-20))
  {
    crosshair->SetAngle(iAngle);
    UpdateAiming();
  }
  
  //Wichtig: Waffe updaten
  EffectCall(this(),wpneffect,"Timer");
}

private func Fire()
{
  var weapon = Contents();
  if(!weapon) return(false);
  if(!weapon->IsWeapon()) return(false);

  if(IsFiring())
    return(true);
    
  if(weapon->~Fire())
  {
    return(true);
  }
  else
  {
    var ammoid = weapon->GetFMData(FM_AmmoID);
    var ammousage = weapon->GetFMData(FM_AmmoUsage);
    
    if(!CheckAmmo(ammoid,ammousage,weapon))//nicht mehr genügend muni in der waffe?
    {
      if(CheckAmmo(ammoid,ammousage,this()))//der clonk hat noch was?
      {
        weapon->Reload();
      }
    }
  }
  
  return(false);
}

private func StopFire()
{
  if(Contents())
    if(Contents()->~GetFMData(FM_Auto,true))
      Contents()->StopAutoFire();
    
  return(true);
}

private func IsFiring()
{
  var weapon = Contents();
  if(!weapon) return(false);
  if(!weapon->IsWeapon()) return(false);
  
  if(GetEffect("Recharge",weapon))
    return(true);
  return(false);
}

private func HasAmmo(object weapon)//Prüft ob der Clonk Muni zum Nachladen hat.
{
  if(!weapon) weapon = Contents();
  if(!weapon) return(false);
  if(!weapon->IsWeapon()) return(false);

  var ammoid = weapon->GetFMData(FM_AmmoID);
  var ammousage = weapon->GetFMData(FM_AmmoUsage);
  
  if(!CheckAmmo(ammoid,ammousage,weapon))
    if(!CheckAmmo(ammoid,ammousage,this()))
      return(false);
      
  return(true);
}


private func StandUp()
{
  if(IsAiming())
    StopAiming();
}