/*-- Kampfmesser --*/

#strict 2

public func HandX()		{return(6000);}
public func IsEquipment()	{return(true);}
public func IsDrawable()	{return(true);}
private func KnifePower()	{return(45);}
func IsEquipment()		{return(true);}
public func NoArenaRemove()	{return(true);}

local action;


/* Angreifen */

public func ControlThrow()
{
  CloseCombat();
  return true;
}

public func Activate()
{
  CloseCombat();
}

public func CloseCombat()
{
  if(action) return false;

  if(!(GetUser()->ReadyToFire())) return false;

  if(GetAction(GetUser()) == "WalkArmed")
  {
    if(Random(3)) Sound("ClonkMelee*.ogg",0,GetUser());
    Sound("GrenadeThrow*.ogg");
    ObjectSetAction(GetUser(),"Throw");
    ScheduleCall(0,"AttackStart",10);
    action = true;
  }

  return true;
}

private func AttackStart()
{
  var pClonk = GetUser();

  var px,py,dx,dy;
  if(target)
  {
    px = (GetX()-GetX(target))*5/4;
    py = (GetY()-GetY(target))*5/4;
    dx = GetX(target)-GetX();
    dy = GetY(target)-GetY();
  }

  //Feinde suchen
  for(var target in FindObjects(Find_Or(Find_AtPoint(px,py),Find_AtPoint(px/2,py/2)),Find_OCF(OCF_Alive),	//Am Leben?
  						Find_NoContainer(),		//Im Freien?
  						Find_Exclude(pClonk)))		//Nicht der Messerführer selbst?
  if(!(CheckEnemy(target,pClonk)))
    continue;

  //Richtung feststellen
  var dir = +1;
  if(GetDir(pClonk) == DIR_Left)
    dir = -1;

  if(target)
  {
    Sound("CTKF_Stab*.ogg",target,0,100,0,0,Random(3)+1);
    Sound("CTKF_Pull.ogg");
    DoDmg(KnifePower(),DMG_Melee,target);
    ScheduleCall(0,"AttackEnd",30);
    return true;
  }
  else
    if(GBackSolid(dir*10,0))
    {
      Sound("CTKF_WallHit.ogg");
      AddLightFlash(20,dir*5,0,RGB(255,255,180));
      Sparks(Random(3),RGB(255,255,0),dir*5,0);
    }
  ScheduleCall(0,"AttackEnd",20);
}

private func AttackEnd()
{
  action = false;
}

/* Allgemein */

public func GetUser()
{
  return Contained();
}

protected func Hit()
{
  Sound("MetalHit*");
}

protected func Selection()
{
  Sound("CTKF_Deploy.ogg");
}