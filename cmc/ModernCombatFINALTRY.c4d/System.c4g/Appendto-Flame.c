/*-- KillIcons für Flammen --*/

//Tod durch Flammen werden nun mit einem passenden Icon markiert.

#strict 2
#appendto FFLM

local creator;


func Construction(object byObj)
{
  if(byObj) creator = GetID(byObj);
  return _inherited(byObj);
}

func GetKillIcon()
{
  if(creator) return creator;
  return;
}

public func HitObject(object pObj)
{
  var color = FIRE_Green;

  var schwaecher = Max(100 - hits*20,0);

  // Energie abziehen und langsamer machen
  if(!slimy)
  {
    color = FIRE_Red;
    DoDmg(iDamage*schwaecher/100, DMG_Fire, pObj, 1, GetController()+1);
  }
  else
  {
    DoDmg(iDamage*schwaecher/100, DMG_Bio, pObj, 1, GetController()+1);
  }

  hits++;

  //Anzündbares anzünden, aber nicht Lebewesen
  if(pObj)
  {
    if(GetOCF(pObj) & OCF_Inflammable)
    {
      if(!(GetOCF(pObj) & OCF_Living))
      {
        if(!slimy)
          Incinerate(pObj);
      }
      else
      {
        AddFireEffect(pObj,50,color,1);
      }
    }
  }
}