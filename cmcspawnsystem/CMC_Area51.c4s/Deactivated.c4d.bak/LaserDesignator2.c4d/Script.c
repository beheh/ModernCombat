/*-- Lasermarkierer --*/

#strict 2

local reload, marker;

public func HandSize()		{return 1000;}
public func HandX()		{return 4500;}
public func IsDrawable()	{return true;}
public func CanAim()		{return true;}
public func MaxStrikes()	{return 10;}
func IsEquipment()		{return true;}
public func NoArenaRemove()	{return true;}


/* Initialisierung */

public func Initialize()
{
  reload = MaxStrikes(); //Lasermarkierer einsatzbereit
}

public func GetUser()
{
  return Contained();
}

public func Ready()
{
  return reload >= 10;
}

/* Nachladen */

public func Reload()
{
  //Eigentliches "Nachladen"
  reload = BoundBy(reload+1,0,MaxStrikes());
}

/* Anfrage stellen */

public func ControlThrow(pByObject)
{
  if(Use(pByObject))
  {
    SetAction("Reload");
  }
  return true;
}

public func Activate(pClonk)
{
  //Nichts tun
}

func Use(caller)
{
  //Hinweis wenn nicht feuerbereit
  if(reload <= 10)
  {
    PlayerMessage(GetOwner(caller), "$NotAvailable$", Contained(this));
    Sound("RadioConfirm*.ogg");
  }

  //Anforderung abbechen wenn nicht bereit
  if(!Ready()) return false;

  //Nicht in Gebäuden
  if(Contained(caller)) return false;

  var user = GetUser();
  var angle = user->AimAngle();
  var x,y; user->WeaponEnd(x,y);
  var dir = GetDir(user)*2-1;
  var laser = CreateObject(LASR,x,y,GetController(user));
  laser->Set(angle,5,500,25);
  laser->LaserEnd(x,y);
  x+=GetX();
  y+=GetY();
  
  Message("Koordinaten %d/%d",GetUser(),x,y);
  Airstrike(x,y);
  marker = false;

  //Nachricht
  for(var i = 0; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(i) == GetPlayerTeam(GetOwner(caller)))
      EventInfo4K(i+1, Format("$Confirmed$",  GetPlrColorDw(GetOwner(caller)), GetPlayerName(GetOwner(caller))), LSDR, 0, 0, 0, "RadioConfirm*.ogg");

  //Granatwerferteam erstmal beschäftigt
  reload = BoundBy(reload-10,0,MaxStrikes());
  //ScheduleCall(0, "Reload", 20); //35*20

  return true;
}

/* Luftschlag */

global func Airstrike(int iX, int iY)
{
  return(AddEffect("Airstrike",0,30,10,0,0, iX, iY));
}

global func FxAirstrikeStart(object pTarget, int iEffectNumber, int iTemp, x, y)
{
  Sound("Morse");
  EffectVar(0,pTarget,iEffectNumber) = x;
  EffectVar(1,pTarget,iEffectNumber) = y;
}

global func FxAirstrikeTimer (object pTarget, int iEffectNumber, int iEffectTime)
{
  if((iEffectTime >= 35*7) && !EffectVar(2,pTarget,iEffectNumber))
  {
    EffectVar(2,pTarget,iEffectNumber) = true;
    Sound("Airstrike1");
  }

  if(iEffectTime >= 35*8)
  {
    //Ausräuchern!
    CastObjects (TBUL,5,20,EffectVar(0,pTarget,iEffectNumber),-50); 
    return(-1);
  }
}

/* Bereitschaft */

func Ready()
{
  //Nicht wenn am Nachladen
  if(GetAction() == "Reload")
    return false;

  //Nicht bei zu wenig Spannung
  if(reload >= 10)
    return true;
}

public func RejectEntrance(object pObj)
{
  if(GetOCF(pObj) & OCF_Living)
    return ContentsCount(GetID(),pObj);
}

/* HUD */

func CustomHUD()	{return true;}

func UpdateHUD(object pHUD)
{
  pHUD->Charge(reload,MaxStrikes());
  pHUD->Ammo(reload, MaxStrikes(), GetName(), true);
  if(GetAction() == "Reload")
    pHUD->Recharge(GetActTime(), 34);
}

/* Allgemein */

protected func Hit()
{
  Sound("WPN2_Hit*.ogg");
}

protected func Selection()
{
  Sound("CDBT_Charge.ogg");
}