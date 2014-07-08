/*-- NUR OPEN BETA --*/

#strict 2

#appendto RSLH

/* Schlagen */

public func ExecShove()
{
  //Nicht schlagen wenn noch dabei
  if(GetEffect("Shove",this)) return false;

  //Schwingeffekt
  AddEffect("Shove",this,10,ShoveTime(),this);
  Sound("ClonkMelee*.ogg",0,target);

  SetController(GetController(target));

  var px,py,dx,dy;
  if(target)
  {
    px = (GetX()-GetX(target))*5/4;
    py = (GetY()-GetY(target))*5/4;
    dx = GetX(target)-GetX();
    dy = GetY(target)-GetY();
  }

  //Objekt finden
  var victims = FindObjects(Find_OnLine(px, py + GetDefHeight()/2, px/2, 0 - GetDefHeight()/2),
                 Find_And(Find_Exclude(this()), Find_Exclude(target)),
                 Find_NoContainer(),
                 Find_Or
                 (
                   Find_And
                   (
                     Find_Or
                     (
                       Find_Func("IsBulletTarget",GetID(),this,target),
                       Find_OCF(OCF_Alive)
                     ),
                     Find_Category(C4D_Living|C4D_Vehicle),
                     Find_Not(Find_Func("IsHelicopter")),
                     Find_Hostile(GetOwner(this))
                   ),
                   Find_Func("IsMeleeTarget", this)
                 ),
                 Sort_Distance(dx-GetX(target),dy-GetY(target)));

  //Und verschleudern
  for(var victim in victims)
  {
  	if(victim->~HitExclude(this))
  		continue;
  
    //Normaler Schaden
    var iDmg = 15;
    if(victim->~IsCrawling())
    {
      //Erhöhter Schaden
      iDmg = iDmg * 3 / 2;
      //Ziel zum Aufstehen zwingen
      ObjectSetAction(victim, "KneelUp");
    }
    else
      if(!victim->~MeleeHit(this))
        Fling(victim, (GetDir(target)*2-1)*2, -1);
    //Schaden verursachen wenn das Ziel ein Lebewesen ist
    if(GetOCF(victim) & OCF_Living)
    {
      DoDmg(iDmg,DMG_Melee,victim,0,GetController()+1,RSHL);
      //Achievement-Fortschritt (Fist of Fury)
      if(GetOCF(victim) & OCF_CrewMember)
        DoAchievementProgress(1, AC36, GetController());
    }
    Sound("RSHL_Shove.ogg", 0, victim);
  }
  if(!victims)
  {
    //Luft schlagen
    Sound("GrenadeThrow*.ogg");
  }
}
