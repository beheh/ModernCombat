/*-- Laser Ziel --*/

#strict

static const LAST_MaxDist = 500;
local target, type, invalid;

func Initialize()
{
  target = 0;
  type = NONE;
  invalid = false;
  return(1);
}

public func Invalid()
{
  return(invalid);
}

public func Set(object pTarget, id idType)
{
  target = pTarget;
  type = idType;
  SetOwner(GetOwner(pTarget));
  SetAction("Laser");
  Laser();
}

private func Laser()
{
  if(!target)
  {
    RemoveObject();
    return();
  }

  //Position aktualisieren...
  var angle;
  if(target->~GetUser())
    angle = target->GetUser()->~AimAngle();
  else
    angle = target->~AimAngle();
    
  Update(angle-180);
  
  if(!invalid)
  {
    //Etwas rumflackern...
    SetClrModulation(RGBa(255,255,255,Random(84)));
  }
}

private func Update(int iAngle)
{
  var mx = -Sin(iAngle, LAST_MaxDist),
      my = +Cos(iAngle, LAST_MaxDist),
      ml = 0,
      x,y;

  while(++ml)
  {
    if(target->GBackSolid(mx*ml/LAST_MaxDist, my*ml/LAST_MaxDist)) break;
    if(ml >= LAST_MaxDist) break;
  }
  
  x = mx*ml/LAST_MaxDist;
  y = my*ml/LAST_MaxDist;
  
  /*for(var i = 0; i < ml; i = Min(ml,i+3))
  {
    for(var pObj in FindObjects(Find_AtPoint(NotZero(mx*i/ml),my*i/ml),
                                Find_Exclude(this()), Find_Exclude(target), Find_Exclude(Contained(target)), Find_Exclude(target->~GetUser()),
                                Find_NoContainer(),
                                Find_Or
                                (
                                  Find_Func("IsBulletTarget",type,this(),target),
                                  Find_OCF(OCF_Alive)
                                ),
                                Find_Func("CheckEnemy",this())))
    {
      x = NotZero(mx*i/ml);
      y = my*i/ml;
      break;
    }
  }*/

  SetPosition(GetX(target)+x, GetY(target)+y);
  
  if(Distance(x,y) >= LAST_MaxDist)
  {
    SetVisibility(VIS_None);
    invalid = true;
  }
  else
    invalid = false;
}

private func NotZero(int a)
{
  if(!a) return(1);
  else return(a);
}