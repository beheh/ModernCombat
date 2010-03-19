/*-- Abwehrschild --*/

#strict

public func IsDrawable() {return(true);}
public func HandX() {return(0);}
public func HandY() {return(-1200);}
public func HandR() {return(45);}
public func HandSize() {return(850);}
public func CanAim() {return(true);}
func IsEquipment(){return(true);}

local pShield, pUser;


/* Initalisierung */

protected func Initialize()
{
  pShield = 0;
  pUser = 0;
}

/* Besitzerkontrolle */

public func SetUser(object pNewUser)
{
  pUser = pNewUser;
  SetOwner(GetOwner(pUser));
  SetController(GetController(pUser));
}

public func GetUser()
{
  return(pUser);
}

public func CheckChange()
{
  if(pUser)
   if(Contents(0,pUser) != this())
    RemoveShield();
  return(1);
}

/* Steuerung */

public func ControlThrow(caller)
{
  if(pShield)
  {
   pShield->ExecShove();
   return 1;
  }
  return 0;
}

/* Ablegen */

public func Departure()
{
  SetUser();
  RemoveShield();
  pShield = 0;
  pUser = 0;
  SetObjDrawTransform();
}

/* Aufnehmen */

public func Entrance(object pContainer)
{
  //Träger gleich Besitzer
  SetUser(pContainer);

  //Position setzen
  SetR(0);
  SetRDir(0);

  //Schild erstellen wenn aktiv gewählt
  if(pUser->Contents(0) == this)
   if(GetOCF(pContainer) & OCF_CrewMember)
    CreateShield();
}

/* Schilderstellung und -entfernung */

private func CreateShield()
{
  if(!GetUser()) return();
  
  SetAction("Invis");
  
  if(pShield)
  {
   pShield->Set(GetUser(),this);
  }
  else
  {
   pShield = CreateObject(RSLH,0,0,GetOwner(GetUser()));
   pShield->Set(GetUser(),this);
   Sound("RSHL_Deploy.ogg");
  }
}

private func RemoveShield()
{
  //Kein Schild, kein entfernen
  if(!pShield) return();

  SetAction("Idle");
  Sound("RSHL_Hide.ogg");

  //Schild weg
  RemoveObject(pShield);
}

/* Sonstiges */

protected func Hit()
{
  Sound("BlockOff*.ogg");
  return(1);
}

func Selection()
{
  //Schild erstellen
  CreateShield();
}

func Deselection()
{
  //Schild entfernen
  RemoveShield();
}

func Destruction()
{
  RemoveShield();
}