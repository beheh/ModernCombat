/*-- Gezogenes Schild --*/

#strict 2

local target, item, angle, last;

public func NoWarp() {return(true);}
public func ShoveTime() {return 13*3;}


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
  Sound("ClonkAction*.ogg",target);

  var px,py,dx,dy;
  if(target)
  {
   px = (GetX()-GetX(target))*5/4;
   py = (GetY()-GetY(target))*5/4;
   dx = GetX(target)-GetX();
   dy = GetY(target)-GetY();
  }

  //Objekt finden
  var victim = FindObject2(Find_AtPoint(px,py),
                 Find_Exclude(this()),
                 Find_NoContainer(),
                 Find_Or
                 (
                  Find_Func("IsBulletTarget",GetID(),this,target),
                  Find_OCF(OCF_Alive)
                 ),
                 Find_Func("CheckEnemy",this),
                 Sort_Distance(dx,dy));

  //und verschleudern
  if(victim)
  {
   Fling(victim, (target->GetDir()*2-1)*2, -1);
   //Schaden durch Schlag wenn das Ziel ein Lebewesen ist
   if(GetOCF(victim) & OCF_Living)
    DoDmg(15,DMG_Melee,victim);
   Sound("RSHL_Shove.ogg");
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
  {
   item->RemoveShield();
   return;
  }

  var x,y,r;
  if(!target->WeaponAt(x,y,r))
  {
   Hide();
   return;
  }
  else
  {
   Show();
  }

  angle = Normalize(target->AimAngle());
  var dir = target->GetDir()*2-1;

  if(GetEffect("Shove",this))
  {
   var t = GetEffect("Shove",this,0,6);

   if(t > ShoveTime()/3)
    t = 35+((t-35)*2/3);

   angle -= Max(Sin(t*90/(ShoveTime()/3), 40),0) * dir;
  }

  SetVertexXY(0,-Sin(angle,7),+Cos(angle,7));

  //Nur optisch drehen
  var fsin = -Sin(angle,1000), fcos = +Cos(angle,1000);
  SetObjDrawTransform
  (
   dir*-fcos, +fsin, 0,
   dir*+fsin, +fcos, 0
  );
}

/* Trefferverhalten */

public func IsBulletTarget(id idBullet, object pBullet, object pShooter)
{
  if(!pBullet && !pShooter)
   return true;

  if(pBullet)
   if(pBullet->~IsGrenade())
    return true;

  var r;
  if(pBullet)
   r = Angle(GetX(),GetY(),GetX(pBullet),GetY(pBullet));
  else
   r = Angle(GetX(),GetY(),GetX(pShooter),GetY(pShooter));

  r = AngleOffset4K(GetR(),r);

  if(Abs(r) <= 45)
   return false;
  else
   return true;
}

public func OnHit(int iDamage, int iType, object pFrom)
{
  //Effekte
  Sound("BlockOff*.ogg");
  CastParticles("Glas", 1+Random(3), 40, 0,0, 40,10, RGBa(200,200,200), RGBa(2,2,200));
  Sparks(Random(2)+6,RGB(255,255,Random(5)+255));
}

public func QueryCatchBlow(object pObj) 
{
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
   if(GetMass(pObj) < 10)  Sound("ArmorImpact*",false,0,50);
   ProtectedCall(pObj,"Hit");
   last = pObj;
  }
  return(true);
}

/* Schlageffekt */

public func FxShoveTimer() { return -1; }

/* Verstecken und Anzeigen */

private func Hide()
{
  if(ActIdle()) return;

  SetAction("Idle");
  SetPosition(10,10);
  SetR(0);
  SetRDir(0);
  SetXDir(0);
  SetYDir(0);
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
