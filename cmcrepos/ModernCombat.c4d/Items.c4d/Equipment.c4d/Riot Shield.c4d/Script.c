/*-- Abwehrschild --*/

#strict

/* Allgemein */

public func IsDrawable() {return(true);}
public func HandX() {return(0);}
public func HandY() {return(-1000);}
public func HandR() {return(45);}
public func HandSize() {return(850);}
public func CanAim() {return(true);}//Was währen wir ohne CanAim()? ^^
//public func NoCrosshair(){return(1);}

local pShield,pUser;

protected func Initialize()
{
  pShield = 0;
  pUser = 0;
}

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

/* Kontrolle */

public func ControlDigDouble(caller)
{
  SetUser(caller);
  if(pShield)
    RemoveShield();
  else
    CreateShield();
}

public func Departure()
{
  SetUser();
  RemoveShield();
}

public func Entrance(object pContainer)
{
  SetUser(pContainer);
}

/* Schild */

private func CreateShield()
{
  if(!GetUser()) return();
  
  SetAction("Invis");
  
  if(pShield)
  {
    pShield->Set(GetUser());
  }
  else
  {
    pShield = CreateObject(RSLH,0,0,GetOwner(GetUser()));
    pShield->Set(GetUser());
  }
}

private func RemoveShield()
{
  if(!pShield) return();
  
  SetAction("Idle");
  RemoveObject(pShield);
}


/* Aktionen */

private func OnBuckleStart()
{
}

private func OnBuckleEnd()
{
}

private func OnUnbuckleStart()
{
}

private func OnUnbuckleEnd()
{
}

//Helper für Aktionen

public func IsBuckling(){return(GetEffect("IntBuckle",this()));}

public func FxIntBuckleStart()
{
  OnBuckleStart();
  return(-1);
}

public func FxIntBuckleTimer()
{
  OnBuckleEnd();
  return(-1);
}

public func IsUnBuckling(){return(GetEffect("IntUnbuckle",this()));}

public func FxIntUnbuckleStart()
{
  OnUnbuckleStart();
  return(-1);
}

public func FxIntUnbuckleTimer()
{
  OnUnbuckleEnd();
  return(-1);
}