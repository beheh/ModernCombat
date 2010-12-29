/*-- Gezogenes Schild --*/

#strict 2

local target, item, angle, last;

public func NoWarp()		{return true;}
public func ShoveTime()		{return 10*3;}
public func RejectEntrance()	{return true;}
public func IgnoreTracer()	{return true;}
public func BlockTracer()	{return true;}
public func RejectC4Attach()	{return true;}


/* Erstellung */

public func Set(object pTarget, object pItem)
{
  target = pTarget;
  item = pItem;

  SetOwner(GetOwner(pTarget));
  SetController(GetController(pTarget));

  Show();
}

/* Nicht brennbar */

public func Incineration()
{
  Extinguish();
}

/* Schlagen */

public func ExecShove()
{
  //Nicht schlagen wenn noch dabei
  if(GetEffect("Shove",this)) return false;

  //Schwingeffekt
  AddEffect("Shove",this,10,ShoveTime(),this);
  Sound("ClonkMelee*.ogg",0,target);

  var px,py,dx,dy;
  if(target)
  {
    px = (GetX()-GetX(target))*5/4;
    py = (GetY()-GetY(target))*5/4;
    dx = GetX(target)-GetX();
    dy = GetY(target)-GetY();
  }

  //Objekt finden
  var victim = FindObject2(Find_Or(Find_AtPoint(px,py),Find_AtPoint(px/2,py/2)),
                 Find_Exclude(this()),
                 Find_NoContainer(),
                 Find_Or
                 (
                  Find_Func("IsBulletTarget",GetID(),this,target),
                  Find_OCF(OCF_Alive)
                 ),
                 Find_Category(C4D_Living|C4D_Vehicle),
                 Find_Not(Find_ID(BKHK)),
                 Find_Func("CheckEnemy",this),
                 Sort_Distance(dx-GetX(target),dy-GetY(target)));

  //Und verschleudern
  if(victim)
  {
    Fling(victim, (GetDir(target)*2-1)*2, -1);
    //Schaden durch Schlag wenn das Ziel ein Lebewesen ist
    if(GetOCF(victim) & OCF_Living)
      DoDmg(15,DMG_Melee,victim,0,GetController()+1,RSHL);
    Sound("RSHL_Shove.ogg", 0, victim);
  }
  else
  {
    //Luft schlagen
    Sound("GrenadeThrow*.ogg");
  }
}

/* Updatecheck */

public func Update()
{
  if(!target)
    return item->RemoveShield();

  var x,y,r;
  if(!target->WeaponAt(x,y,r))
    return Hide();
  else
    Show();

  angle = Normalize(target->AimAngle());
  var dir = GetDir(target) * 2 - 1;

  if(GetEffect("Shove", this))
  {
    var t = Sin(GetEffect("Shove", this, 0, 6) * 90 / ShoveTime(), ShoveTime());
    angle -= dir * Sin(t * 180 / ShoveTime(), 40);
  }

  SetVertexXY(0, -Sin(angle, 7), Cos(angle, 7));

  //Nur optisch drehen
  var fsin = -Sin(angle, 1000), fcos = Cos(angle, 1000);
  SetObjDrawTransform(dir * -fcos, fsin, 0, dir * fsin, fcos);
}

/* Trefferverhalten */

public func IsBulletTarget(id idBullet, object pBullet, object pShooter, int oldx, int oldy)
{
  if(GetAction() != "Attach")
    return;

  if(!pBullet && !pShooter)
    return true;

  if(pBullet)
    if(pBullet->~IsGrenade())
      return true;

  var r;
  if(pBullet && oldx && oldy)
    r = Angle(GetX(),GetY(),oldx,oldy);
  else
    r = Angle(GetX(),GetY(),GetX(pShooter),GetY(pShooter));

  r = AngleOffset4K(angle,r);

  if(Abs(r) > 45)
    return false;
  else
    return true;
}

public func OnHit(int iDamage, int iType, object pFrom)
{
  if(Hostile(GetController(pFrom), GetController())) item->DoHit(iDamage);
  //Effekte
  Sound("MetalHit*.ogg");
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Glas", 1+Random(3), 40, 0,0, 40,10, RGBa(200,200,200), RGBa(2,2,200));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) if(!Random(5)) Sparks(Random(2)+6,RGB(255,255,Random(5)+255));
}

public func QueryCatchBlow(object pObj) 
{
  if(GetAction() != "Attach")
    return;
  if(pObj == last)
    return;
  if(!Hostile(GetController(pObj),GetController(this)))
    return;

  var iPower = BoundBy(GetMass(pObj),0,50) * Distance(GetXDir(pObj),GetYDir(pObj)) * Distance(GetXDir(pObj),GetYDir(pObj));
  if(iPower/3 < 30000)
  {
    //if(GetXDir(pObj) < 0) if(GetX(pObj) < GetX() + 6) SetPosition(GetX() + 9, GetY(pObj) - GetYDir(pObj) / 3, pObj); 
    //if(GetXDir(pObj) > 0) if(GetX(pObj) > GetX() - 6) SetPosition(GetX() - 9, GetY(pObj) - GetYDir(pObj) / 3, pObj);
    //Hochkomplizierte Berechnungen
    var rad = Distance(GetX(),GetY(),GetX()+GetXDir(pObj),GetY()+GetYDir(pObj));
    var enterAngle = Angle(GetX(),GetY(),GetX()+GetXDir(pObj),GetY()+GetYDir(pObj))-target->AimAngle();
    var exitAngle = -enterAngle+target->AimAngle()+180;
    SetXDir(Sin(exitAngle,rad)*2/3, pObj);
    SetYDir(-Cos(exitAngle,rad)*2/3, pObj); 

    if(GetMass(pObj) >= 10) Sound("ClonkHit*");
    if(GetMass(pObj) < 10)  Sound("RSHL_Shove.ogg",false,0,50);
    ProtectedCall(pObj,"Hit");
    last = pObj;
  }
  return true;
}

/* Schlageffekt */

public func FxShoveTimer()	{return -1;}

/* Verstecken und Anzeigen */

private func Hide()
{
  if(ActIdle()) return;

  SetAction("Idle");
  SetCategory(C4D_StaticBack);
  SetVisibility(VIS_None);
}

private func Show()
{
  if(!ActIdle()) return;

  SetAction("Attach",target);
  SetPosition(GetX(target),GetY(target));
  SetCategory(GetCategory(0,GetID()));
  SetVisibility(VIS_All);
  Update();
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Bio)		return 100;	//S�ure und biologische Schadstoffe
}