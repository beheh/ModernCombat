/*-- Phosphor --*/

#strict 2

local light,last_size,life;


/* Initialisierung */

func Initialize()
{
  //Brennzeit setzen
  life = 300+Random(100);

  //Effekte
  light = AddLight(40,RGB());
  AddFireEffect(this,0,RGB(0,255,255),0,-5 - Random(15));

  //Besitzer setzen und Timer ausführen
  SetController(GetOwner());
  Timer();

  return 1;
}

/* Aktivität */

func Timer()
{
  //Auswerfen wenn verschachtelt
  if(Contained())
    Exit(this, GetX(Contained())-GetX(), GetY(Contained())-GetY());

  //Geschwindigkeit bestimmen
  var speed = Distance(GetXDir(),GetYDir());
  speed += Random(3);

  if(!(GetActTime() % 5))
  {
    speed = BoundBy (speed,1,40);
    speed = (255/40)*speed;	//Erhöhen von 0 auf 255

    //Färbung
    var rgb = RGB(speed,BoundBy(speed,140,255),255);
    SetClrModulation(rgb);
    if(light)
      light->ChangeColor(rgb);
  }

  //Ziele suchen und verbrennen
  BurnObjects();

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
    if(!Random(20))
      CastObjects(SPRK, 1, 20);

  //Brennzeit verwalten
  life--;
  if(GBackLiquid())
  {
    CastObjects(FXU1, Random(3),10);
    life -= 10;
  }
  if(life <= 0)
    RemoveObject();
}

/* Objekte verbrennen */

func BurnObjects()
{
  //Objekte suchen
  for(var pTarget in FindObjects(Find_Distance(10), Find_Not(Find_ID(GetID())), Find_NoContainer(), Find_OCF(OCF_Inflammable), Find_Or(Find_Func("IsBulletTarget", GetID(), this), Find_OCF(OCF_Prey))))
  {
    if(!pTarget)
      continue;
    //Objekt verbrennen
    HitObject(pTarget);
  }
}

func HitObject(pObj)
{
  //Lebenwesen mit Effekt belegen
  if(GetOCF(pObj) & OCF_Living)
    AddEffect("Phosphored", pObj, 50, 20, this, GetID());

  //Schaden verursachen
  DoDmg(3, DMG_Fire, pObj, 1);
  AddFireEffect(pObj,50,FIRE_Red,1);

  //Anzündbares anzünden, aber nicht Lebewesen
  if(pObj)
    if(GetOCF(pObj) & OCF_Inflammable && !(GetOCF(pObj) & OCF_Living))
      Incinerate(pObj, GetController()+1);

  //Effekte
  if(!Random(20))
    Sound("Crackle.ogg",0,0,RandomX(20,40));

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
  EffectVar(0, pTarget, iNo) = this;
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