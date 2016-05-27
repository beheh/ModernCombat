/*-- Kampfmesser --*/

#strict 2

local readytoknife;

public func IsDrawable()		{return true;}			//Wird sichtbar getragen
public func HandX()			{return(6000);}

public func IsEquipment()		{return true;}
public func NoArenaRemove()		{return true;}
public func CustomQuickInventoryMenu()	{return QINV_ObjectMenu;}


/* Initialisierung */

public func Initialize()
{
  readytoknife = true;
}

/* Angriff */

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
  if(!Ready()) return;
  if(!(GetUser()->ReadyToFire())) return;

  if(GetAction(GetUser()) == "JumpArmed")
  {
    //Sofortiger, schwächerer Angriff in der Luft
    Attack(25);
    readytoknife = false;

    //Effekte
    Sound("GrenadeThrow*.ogg");
  }
  else
  if(GetAction(GetUser()) == "WalkArmed")
  {
    //Verzögerter, stärkerer Angriff am Boden
    ScheduleCall(0,"Attack",10,0,40);
    readytoknife = false;

    //Feind suchen
    var dir = GetDir(GetUser());
    for(var target in FindObjects(Find_InRect(-15+10*dir,-10,20,20),Find_OCF(OCF_Alive),	//Am Leben?
  						Find_NoContainer(),				//Im Freien?
  						Find_Exclude(GetUser())))			//Nicht der Angreifer selbst?
    if(!(CheckEnemy(target,GetUser())))
      continue;

    if(target)
    {
      //Umwerfen
      ObjectSetAction(target,"Tumble");
      Fling(target,0,-1);

      Sound("ClonkImpact*.ogg",0,target);
      Sound("ClonkRustle*.ogg",0,target);
    }

    //Effekte
    ObjectSetAction(GetUser(),"Throw");
    Sound("ClonkMelee*.ogg",0,GetUser());
    Sound("GrenadeThrow*.ogg");
  }

  return true;
}

private func Attack(int power)
{
  var pClonk = GetUser();
  if(!(pClonk->ReadyToFire()))
  {
    SetAction("ReloadFast");
    ScheduleCall(0,"AttackEnd",25);
    return;
  }

  //Feind suchen
  var dir = GetDir(GetUser());
  for(var target in FindObjects(Find_InRect(-15+10*dir,-10,20,20),Find_OCF(OCF_Alive),	//Am Leben?
  						Find_NoContainer(),			//Im Freien?
  						Find_Exclude(pClonk)))			//Nicht der Angreifer selbst?
  if(!(CheckEnemy(target,pClonk)))
    continue;

  if(target)
  {
    //Schaden verursachen
    DoDmg(power,DMG_Melee,target);

    //ObjectSetAction(target,"Tumble");
    //Fling(target,0,-1);

    //"Nachladen"
    SetAction("Reload");
    ScheduleCall(0,"AttackEnd",50);

    //Effekte
    Sound("CTKF_Stab*.ogg",target,0,100,0,0,Random(3)+1);
    Sound("CTKF_Pull.ogg");

    return true;
  }
  else
    if(GBackSolid(dir*10,0))
    {
      Sound("CTKF_WallHit.ogg");
      AddLightFlash(20,dir*5,0,RGB(255,255,180));
      Sparks(Random(3),RGB(255,255,0),dir*5,0);
    }

  //"Nachladen"
  SetAction("ReloadFast");
  ScheduleCall(0,"AttackEnd",25);
}

private func AttackEnd()
{
  readytoknife = true;
}



/* Bereitschaft */

func Ready()
{
  //Nicht bei Anwahlverzögerung
  if(GetEffect("IntSelection", this))
    return false;

  //Nicht wenn am Nachladen
  if(GetAction() == "Reload" || GetAction() == "ReloadFast" || !readytoknife)
    return false;

  return true;
}





/* HUD */

public func CustomHUD()	{return true;}
public func MinValue()	{return 9;}
public func MinValue2()	{return 19;}

func UpdateHUD(object pHUD)
{
  pHUD->~Charge(readytoknife, 1);
  pHUD->~Ammo(readytoknife, 1, GetName(), true, RGB(255, 255, 255));
  if(GetEffect("IntSelection", this) != 0)
    pHUD->Recharge(GetEffect("IntSelection", this, 0, 6), GetEffect("IntSelection", this, 0, 3)-1);
  if(GetAction() == "Reload")
    pHUD->Recharge(GetActTime(), 49);
  if(GetAction() == "ReloadFast")
    pHUD->Recharge(GetActTime(), 24);
}





/* Allgemein */

protected func Selection()
{
  //Anwahlverzögerung
  AddEffect("IntSelection", this, 1, 25, this);

  Sound("CTKF_Deploy.ogg");
}

public func Entrance(object pContainer)
{
  if(Contents(0, pContainer) == this)
  {
    //Anwahlverzögerung
    AddEffect("IntSelection", this, 1, 25, this);

    Sound("CTKF_Deploy.ogg");
  }
}




public func GetUser()
{
  return Contained();
}

protected func Hit()
{
  Sound("MetalHit*");
}