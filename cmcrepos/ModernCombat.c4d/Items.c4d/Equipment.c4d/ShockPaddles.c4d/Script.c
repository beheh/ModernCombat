/*--- Defibrillator ---*/

#strict

local charge;

public func HandSize() {return(1000);}
public func HandX() {return(4500);}
public func HandY() {return(0);}
public func IsDrawable() {return(true);}
public func IsDrobotMaterial(){return(true);}

public func MaxEnergy() {return(30);}

func IsEquipment(){return(true);}

public func Initialize()
{
  charge = MaxEnergy();//geladen
}

public func GetUser()
{
  return(Contained());
}

public func Timer()
{
  if(GetUser())
    if(GetUser()->~IsReanimating())
      return(false);
      
  charge = BoundBy(charge+1,0,MaxEnergy());//TODO: Ladegschwindigkeit.
      
  return(true);
}

public func ControlThrow()
{
  Use();
  return(true);
}

public func ControlUp(object pClonk)
{
  if(GetPlrDownDouble(GetController(pClonk)))
  {
    pClonk->~StartHeal(MDIC_HealMode_Reanimate);
    return(true);
  }
  return(false);
}

public func Activate(pClonk)
{
  return(Use());
}

func Use()
{
  if(!Ready()) return(false);

  var dir = +1;
  if(GetDir(GetUser()) == DIR_Left)
    dir = -1;
    
  if(GetUser()->~Reanimate()) return(1);
  
  if(!GetUser()->~IsMedic())  return(HelpMessage(GetOwner(GetUser()),"$OnlyMedic$",GetUser()));
  
  var obj;
  if(GetUser()->Contained())
  {
    if(GetOCF(GetUser()->Contained()) & OCF_Alive)
      obj = GetUser()->Contained();
  }
  else
  {
    obj = FindObject2(Find_Or(Find_AtPoint(15*dir,0),Find_InRect(0,-10,40*dir,20)),
                      Find_OCF(OCF_Living),Find_Exclude(GetUser()));
  }
  
  if(obj && CheckEnemy(GetUser(),obj))
  {
    if(GetEnergy(obj) <= 160)
    {
      if(!Random(2))//TODO: Skill kann hier ansetzen.
        if(!FakeDeath(obj))
          Fling(obj,0,-1);
      else
        Fling(obj,0,-1);//Wahahaha!
    }
    //Schaden tun.
    DoDmg(30+Random(10),DMG_Energy,obj);
    
    if(!obj)//Könnte ja jetzt weg sein.
      obj = this();
        
    //Effekgehasche.
    Sound("Defibrillator");
    obj->CastObjects(SPSK,5,20);
    obj->AddLightFlash (40+Random(20),0,0,RGB(0,140,255));
    
    //Energie abziehen.
    charge = BoundBy(charge-20,0,60);
  }
  else
  {
    //Effekgehasche.
    Sound("DefibrillatorFail");
    CastObjects(SPSK,5,20);
    AddLightFlash (40+Random(20),0,0,RGB(0,140,255));
    
    //Energie abziehen.
    charge = BoundBy(charge-10,0,MaxEnergy());//Aber so nur 10 Einheiten. ;)
  }
 
  SetAction("Reload");
  
  return(true);
}

func UseReanimation(pTarget)
{
  if(!Ready())
  {
    Sound("DefibrillatorFail");
    return(false);
  }
  
  Sound("Defibrillator");
  pTarget->CastObjects(SPSK,5,20);
  pTarget->AddLightFlash (40+Random(20),0,0,RGB(0,140,255));
  charge = BoundBy(charge-20,0,60);
  SetAction("Reload");
  return(true);
}

func Ready()
{
  if(GetAction() eq "Reload")
    return(false);

  //Nagut! Jeder kann das. So.
  //if(GetID(GetUser()) == MDIC)
    if(charge >= 20)
      return(true);
  return(false);
}

func GetCharge()
{
  return(charge*100/MaxEnergy());
}

func UpdateCharge(object pHUD)
{
  pHUD->Charge(charge*100/MaxEnergy());
  pHUD->Ammo(charge, MaxEnergy(), GetName(), true);
}

public func RejectEntrance(object pObj)
{
  if(GetOCF(pObj) & OCF_Living)
  {
    if(ContentsCount(GetID(),pObj))//fail. :c
      return(true);
  }
  return(false);
}
