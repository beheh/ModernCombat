/*-- Phosphor --*/

#strict 2

local light,last_size,life;


/* Initialisierung */

func Initialize()
{
  life = 300+Random(100);
  light = AddLight(40,RGB());
  AddFireEffect(this,0,RGB(0,255,255),0,-5 - Random(15));
  SetController(GetOwner());
  Timer();
  return 1;
}

/* Aktivität */

func Timer()
{
  //Steckt irgendwo drin: Raus da
  if(Contained())
    Exit(this, GetX(Contained())-GetX(), GetY(Contained())-GetY());

  var speed = Distance(GetXDir(),GetYDir());
  speed += Random(3);

  //var size = (50/40)*speed+20;//Wir dehen das ganze auf 20-70 aus.
  //size = BoundBy (size,3,70);

	if(!(GetActTime() % 5))
	{
  	speed = BoundBy (speed,1,40);
  	speed = (255/40)*speed;//Wir dehen das ganze auf 0-255 aus.

  	var rgb = RGB(speed,BoundBy(speed,140,255),255);

  	SetClrModulation(rgb);
  	if (light)
    	light->ChangeColor(rgb);
  }
  /*if(last_size != size)
    light->ChangeSize (size);

  last_size = size;*/

  BurnObjects();

  if(GetEffectData(EFSM_ExplosionEffects) > 0) 
  {
    if(!Random(20))
      CastObjects(SPRK, 1, 20);
  }

  life--;

  if(GBackLiquid())
  {
    CastObjects(FXU1, Random(3),10);
    life -= 10;
  }

  if(life <= 0)
    RemoveObject();
}

func BurnObjects()
{
  for(var pTarget in FindObjects(Find_Distance(10), Find_Not(Find_ID(GetID())), Find_NoContainer(), Find_Or(Find_Func("IsBulletTarget", GetID()), Find_OCF(OCF_Prey))) )
  {
    //könnte ja sein, dass jemand in "IsBulletTarget" das Objekt löscht o_O
    if(!pTarget)
      continue;
    //Treffen!
    HitObject(pTarget);
  }
}

func HitObject(pObj)
{
  if(GetOCF(pObj) & OCF_Living)
    AddEffect("Phosphored", pObj, 50, 20, this, GetID());
  DoDmg(3, DMG_Fire, pObj, 1);
  AddFireEffect(pObj,50,FIRE_Red,1);
  //Anzündbares anzünden, aber nicht Lebewesen
  if(pObj) //existiert es überhaupt noch?
    if(GetOCF(pObj) & OCF_Inflammable && !(GetOCF(pObj) & OCF_Living))
      Incinerate(pObj, GetController()+1);
  return true;
}

/* Kleben am Clonk (Hazard) */

protected func Hit()
{
  var contact = GetContact(0, -1, 0);
  if(contact & CNAT_Left || contact & CNAT_Right || contact & CNAT_Top)
    SetAction("Globbing");
  if(life < 300)
    SetAction("Globbing");
}

/* Klebeeffekt */

public func FxPhosphoredStart(pTarget, iNo, iTemp, pPhosphor)
{
  if(iTemp)
    return -1;
  if(GetLength(FindObjects(Find_ID(GetID()), Find_ActionTarget(pTarget))) > 3)
    return -1;
    EffectVar(0, pTarget, iNo) = this; //Der Klumpen
  SetAction("AGlobbing", pTarget);
}

public func FxPhosphoredTimer(pTarget, iNo, iTime)
{
  if(!GetAlive(pTarget) || Contained(pTarget))
    AttachTargetLost();

  if(iTime >= 60)
    return -1;

  if(iTime >= 40)
    AttachTargetLost();
}

protected func AttachTargetLost()
{
  SetAction("Idle");
  SetXDir(RandomX(-20,20));
  SetYDir(RandomX(10,-30));
}
