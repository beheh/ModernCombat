/*---- MetroCop ----*/

#strict
#include PCMK

/* Allgemeines */
public func Faction() {return(FACTION_Combine);}

protected func Initialize()
{
  //InitCombineAI();
  //GiveWeapon(H2PW);

  if(GetOwner() == NO_OWNER)
  {
    SetName(GetName(0,GetID()));
    AddEffect("IntAI",this(),200,20,this());
  }
    
  aCmdData = CreateArray(7);
  bCmd = false;
  SetCmd();
  
  inherited();
}

/* Geräusche */

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

protected func Death()
{
  var plr = GetOwner();

  // Sound und Meldung
  Sound("MCOP_Die*.ogg");
  DeathAnnounce(GetOwner(),this(),GetKiller());
	NoDeathAnnounce();
  
  if(GetPlayerType(GetOwner()) == C4PT_Script)
    GameCallEx("RelaunchPlayer",GetOwner(),this(), GetKiller());
  else
  {
    var gotcrew, plr = GetOwner();
    for(var i; i < GetCrewCount(plr); i++)
      if(GetOCF(GetCrew(plr,i)) & OCF_Alive)
        gotcrew = true;

    if(!gotcrew)
      GameCallEx("RelaunchPlayer",GetOwner(),this(), GetKiller());
  }

  if(ammobag)
    RemoveObject(ammobag);

  if(GetEffect(0,0,wpneffect))
    RemoveEffect(0,0,wpneffect);

  FadeOut(this());

  if(HasGear()) TakeOffGear();

  return(1);
}

/* SetCmd */

local aCmdData,bCmd,bCmdUntilEnemy;

//Kommando das im "Idle"-Modus ausgeführt wird.
public func SetCmd(bool bUntilEnemy, string szCommand, object pTarget, int iX, int iY, object pTarget2, Data, int iRetries)
{
  bCmdUntilEnemy = bUntilEnemy;
  aCmdData[0] = szCommand;
  aCmdData[1] = pTarget;
  aCmdData[2] = iX;
  aCmdData[3] = iY;
  aCmdData[4] = pTarget2;
  aCmdData[5] = Data;
  aCmdData[6] = iRetries;
}


/* Alarmierung */

private func Alert(object pTarget)
{
  if(!pTarget) return();

  Sound("MCOP_Alert*.ogg");
  for(var combine in FindObjects(Find_InRect(-200,-200,+400,+400),Find_Exclude(this()),Find_OCF(OCF_Alive),Find_Faction(Faction())))
    combine->~OnAlert(pTarget);
}

public func OnAlert(object pTarget)
{
  if(pTarget->~Faction() == Faction())
  {
    if(!enemy)
    {
      StandUp();
      SetCmd(false,"Follow",pTarget);
    }
  }
  else
    if(!enemy)
      enemy = pTarget;
}

protected func ContextAlert(object pCaller)
{
  [$Alert$|Image=INFO]
  Alert(this());
}


/* integrierte KI */

static const MCOP_SightRange = 300;
static const MCOP_HighPace = 10;
static const MCOP_LowPace = 35;

public func FxIntAITimer(object pTarget, int iEffectNumber)
{
  AI();
}

public func SetAIPace(int iPace)
{
  if(GetEffect("IntAI",this(),0,3) == iPace) return();
  ChangeEffect("IntAI",this(),0,"IntAI",iPace); 
}

local enemy;

public func AI()
{
  if(!GetAlive()) return();
  
  CheckContents();
  CheckAvoid();
  
  if(!Combat())
  {
    Idle();
    SetAIPace(MCOP_LowPace);
  }
  else
  {
    SetAIPace(MCOP_HighPace);
    if(bCmd)
    {
      bCmd = false;
      if(bCmdUntilEnemy) SetCmd();
    }
  }
}

private func Combat()
{
  //Haben wir einen Gegner?
  if(enemy)
  {
    if(!enemy->GetAlive() || (ObjectDistance(this(),enemy) > MCOP_SightRange) || Contained(enemy))
      enemy = 0;
  }
  
  if(!enemy)
  {
    enemy = FindEnemy();
    
    if(enemy)
    {
      Alert(enemy);
    }
    else
      return(false);
  }


  //Können wir angreifen?
  if((ObjectDistance(enemy,this()) > GetAttackDistance()) || !PathFree(GetX(),GetY(),GetX(enemy),GetY(enemy)))
  {
    MoveToEnemy();
    return(true);
  }
  
  if(!Ready() || !IsArmed())
  {
    Avoid(enemy);
    if(!Random(90))
    {
      var aText = [":I","omg","._.","O_o","O_O'","o_O","gnade!11"];
      ChatMessage(aText[Random(GetLength(aText))]);
    }
    return(true);//Er soll nicht Idle ausführen. :X
  }
    
    
  //Angriff!
  TurnToObject(enemy);
  
  //Müssen wir zielen?
  if(CanAim() && ((Abs(GetY(enemy)-GetY()) > 10) || Contents()->GetFMData(FM_Aim)))
  {
    if(Random(4))
      return(true);
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
        StopMoving();
      }
    }
  }
  
  return(true);
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
  var angle = 90;
  if(GetDir() == DIR_Left) angle = 270;
  var targets = FindTargets(this(),MCOP_SightRange,60,angle);
  
  for(var target in targets)
    if(!Contained(target))
      return target;
}

private func Idle()
{
  if(bCmd) return();

  if(aCmdData[0])
  {
    bCmd = true;
    SetCommand(this(),aCmdData[0],aCmdData[1],aCmdData[2],aCmdData[3],aCmdData[4],aCmdData[5],aCmdData[6]);
    return();
  }

  if(!Random(120))
  {
    var aText = ["-.-","*schnarch*","...","Los! Ich will rumballern.","Kommt raus ihr Angsthasen!","O-o Nix los hier?","*gähn*","Combine rulez!","OMG bin ich toll! :>","Kommt her!|Ich tu euch nix. =D"];
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
  {
    FireChat();
    return(true);
  }
    
  if(weapon->~Fire())
  {
    FireChat();
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

private func FireChat()
{
  if(!Random(90))
  {
    var aText = ["*hrhr*","peng peng","pwnage!",">:D"];
    ChatMessage(aText[Random(GetLength(aText))]);
  }
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