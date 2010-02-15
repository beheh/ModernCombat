/*-- Phosphor --*/

#strict

local light,last_size,life;


/* Initalisierung */

func Initialize()
{
  life = 300+Random(100);
  light = AddLight (40,RGB());
  AddFireEffect(this(),0,RGB(0,255,255),0,-5 - Random(15));
  Timer();
  return(1);
}

/* Aktivität */

func Timer()
{
  var speed = Distance(GetXDir(),GetYDir());
  speed += Random(3);

  //var size = (50/40)*speed+20;//Wir dehen das ganze auf 20-70 aus.
  //size = BoundBy (size,3,70);

  speed = BoundBy (speed,1,40);
  speed = (255/40)*speed;//Wir dehen das ganze auf 0-255 aus.

  var rgb = RGB(speed,BoundBy(speed,140,255),255);

  SetClrModulation (rgb);
  light->ChangeColor (rgb);
  /*if(last_size != size)
    light->ChangeSize (size);

  last_size = size;*/

  var aObjects = CreateArray();
  aObjects = FindObjects (Find_AtPoint(),Find_NoContainer());

  BurnObjects();

  if(!Random(20))
     CastObjects(SPRK, 1, 20);

  life--;

  if(GBackLiquid ())
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
    DoDmg(5, DMG_Fire, pObj, 1);
    AddFireEffect(pObj,50,FIRE_Red,1);
    //anzündbares anzünden, aber nicht lebewesen.
    if(pObj) //existiert es überhaupt noch?
      if(pObj->GetOCF() & OCF_Inflammable && !(pObj->GetOCF() & OCF_Living))
      Incinerate(pObj);
}