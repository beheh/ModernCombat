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
public func MaxDmg(){return(200);}
func IsEquipment(){return(true);}

local pShield, pUser;

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

public func ControlThrow(caller)
{
  if(pShield)
  {
    pShield->ExecShove();
    return 1;
  }
  return 0;
}

public func ControlDigDouble(caller)
{
  SetUser(caller);
  if(pShield)
    RemoveShield();
  else
    CreateShield();
    
  return 1;
}

public func Departure()
{
  SetUser();
  RemoveShield();
  SetObjDrawTransform();//Reset.
}

public func Entrance(object pContainer)
{
  SetUser(pContainer);
  
  if((GetDamage() > 0) && !GetEffect("Repair",this))
    AddEffect("Repair",this,10,14,this);
    
  SetR(0);
  SetRDir(0);
}

/* Schild */

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
  RemoveEffect("Repair",this);
}

private func RemoveShield()
{
  if(!pShield) return();
  
  SetAction("Idle");
  RemoveObject(pShield);
  Sound("RSHL_Deploy.ogg");
  Repair();
}

protected func Hit()
{
  Sound("BlockOff*.ogg");
  return(1);
}

func Selection()
{
  Sound("RSHL_Charge.ogg");
  Repair();
}

func Deselection()
{
  RemoveShield();
}

func Destruction()
{
  RemoveShield();
}


/* Schaden */

public func UpdateDmg(int iDamage, int iBy)
{
  SetKiller(iBy,this);
  DoDamage(iDamage,this);

  return GetDamage() >= MaxDmg();
}

public func Damage(int iChange, int iByPlayer)
{
  if(GetDamage() >= MaxDmg())
  {
    RemoveObject();
    return;
  }

  UpdateGraphics();
}

private func UpdateGraphics()
{
  var p = Max(0,MaxDmg()-GetDamage());
  
  if(p < MaxDmg()/3)
  {
    SetGraphics("Damaged2");
    return;
  }
  
  if(p < MaxDmg()/3*2)
  {
    SetGraphics("Damaged");
    return;
  }
  
  SetGraphics();
}

public func Repair()
{
  if(!GetDamage() || pShield) return;

  if(!GetEffect("Repair",this))
    AddEffect("Repair",this,10,10,this);
}

public func FxRepairTimer()
{
  if(!Contained())
    return -1;

  var dmg = GetDamage();
  if(dmg <= 0)
    return -1;
  
  DoDamage(-1);
}


/* HUD */

func CustomHUD(){return(true);}
func UpdateHUD(object pHUD)
{
  var p = Max(0,MaxDmg()-GetDamage());
  pHUD->Charge(p,MaxDmg());
  pHUD->Ammo(p,MaxDmg(),GetName(),true);
}