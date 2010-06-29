/* Global Scripts */

#strict


/* ----- Verschiedene Ziele finden ----- */

// verfeindetes Crewmitglied
global func GetTargetCrew(int Range, int posX, int posY, object obj2) 
{
  var obj;
  while (1)
  {
    obj = FindObject(0, posX,posY,-1,-1, OCF_CrewMember(),0,0,NoContainer(),obj);
    if (!obj) break;
    if (Distance(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)) > Range) return(0);
    if (Hostile(GetOwner(),GetOwner(obj)) || (GetOwner(obj2)==-1 && GetOwner(obj2)!=GetOwner(obj)))
      if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
        if (!GetCrewMemberInWay(ObjectDistance(obj, this()), 0, 0, Angle(GetX()+posX, GetY()+posY, GetX(obj), GetY(obj))))
          return(obj);
  }
  return(0);
}

// gegnerisches Fahrzeug
global func GetTargetVehicle(int Range, int posX, int posY, object obj2)
{
  var obj;
  while (1)
  {
    obj = FindObject(0, posX,posY,-1,-1, OCF_Entrance(),0,0,NoContainer(),obj);
    if (!obj) break;
    if (Distance(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)) > Range) return(0);
    if (GetCategory(obj) & C4D_Vehicle())
      if (Hostile(GetOwner(),GetOwner(obj)) || (GetOwner(obj2)==-1 && GetOwner(obj2)!=GetOwner(obj)))
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
          if (!GetCrewMemberInWay(ObjectDistance(obj, this()), 0, 0, Angle(GetX()+posX, GetY()+posY, GetX(obj), GetY(obj))))
            if (GetID(obj) != H_HW)
              return(obj);
  }       
  return(0);
}

// gefährliche Tiere
global func GetTargetAnimal(int Range, int posX, int posY, object obj2)     
{
  var obj;
  while (1)
  {
    obj = FindObject(0, posX, posY,-1,-1, OCF_Alive(),0,0,NoContainer(),obj);
    if (!obj) break;
    if (Distance(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)) > Range) return(0);
    if (IsAutoCannonAim(GetID(obj)))
      if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
        if (!GetCrewMemberInWay(ObjectDistance(obj, this()), 0, 0, Angle(GetX()+posX, GetY()+posY, GetX(obj), GetY(obj))))
          return(obj);
  }
  return(0);
}

global func IsAutoCannonAim(id ID)
{
  if (ID == SHRK) return(true);
  if (ID == MONS) return(true);
  if (ID == FMNS) return(true);
  if (ID == SHKE) return(true);
  if (ID == OZRK) return(true);
  return(false);
}

global func GetCrewMemberInWay(int MaxDistance, int X, int Y, int SearchAngle)
{
  if (ObjectCount(NFDF))
    return(false);
  var obj;
  while (obj = FindObject(0, X, Y, -1, -1, OCF_CrewMember, 0, 0, NoContainer(), obj))
  {
    if (Distance(GetX()+X, GetY()+Y, GetX(obj), GetY(obj)) > MaxDistance)
      return(false);
    if (Inside(Angle(GetX()+X, GetY()+Y, GetX(obj), GetY(obj)), SearchAngle-4, SearchAngle+4))
      if (!Hostile(GetOwner(), GetOwner(obj)))
        return(obj);
  }
}

