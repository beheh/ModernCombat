/*-- Target --*/

#strict 2

protected func Initialize()
{
  SetAction("Stand");
}

func Incineration(int iPlr)//Sollte überladen und angepasst werden.
{
  Extinguish();
  SetClrModulation(RGB(70,70,70));
  //Schedule("DoDmg(5,DMG_Fire,0,1)",1,20,this());
  AddFireEffect(this,30,FIRE_Red,1);
  
  if(iPlr > NO_OWNER)
    if(GetDamage() >= MinDmg())
      Activate(iPlr);
}

public func MinDmg(){return 1;}

public func IsBulletTarget()
{
  if(GetAction() == "Stand")
    return true;
  return false;
}	

public func OnHit(int iDamage, int iType, object pFrom)
{
  if(iDamage >= MinDmg())
    Activate(GetController(pFrom));
  else
    ObjectHit(GetController(pFrom));
}

public func Damage(int iChange,int byPlr)
{
  if(!IsBulletTarget()) return ;
  
  if(GetDamage() >= MinDmg())
    Activate(byPlr);
  else
    ObjectHit(byPlr);
    
  return 1;
}

public func ObjectHit(int iPlr)
{

}

public func Activate(int iPlr)
{
  Message("!",this);
  Sound("Ding");
  SetAction("Fall");
  Sound("DOR2_Open");
  DoWealth(iPlr,+10);
}

public func Reset()
{
  if((GetAction() != "Stand")
   ||(GetAction() != "SetUp"))
    return SetAction("SetUp");
  return false;
}

private func Down()
{
  Sound("WoodHit*");
}
