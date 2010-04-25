#strict

//Freundliche KIs nicht abballern.
global func CheckEnemy(object pObj,object pObj2)
{
  if(GetAI(pObj) || GetAI(pObj2)) return(0);
  
  if(!pObj2)
    pObj2 = this();
  var liv1 = (GetCategory(pObj) & C4D_Living),
      liv2 = (GetCategory(pObj2) & C4D_Living);
      
  //ist pObj ein Lebewesen und lebt es noch?
  if(liv1 && !(GetOCF(pObj) & OCF_Alive))
    return(0);

  //und pObj2? 
  if(liv2 && !(GetOCF(pObj2) & OCF_Alive))
    return(0);

  //eines der beiden Lebewesen?
  if(liv1 || liv2)
    //dann können wir ja sowieso abschießen, wenn FriendlyFire an
    if(FriendlyFire())
      return(1);

  //sonst noch auf Verfeindung checken
  if(Hostile(GetController(pObj),GetController(pObj2)))
    return(1);
    
  // noch hackiger gehts nicht oder?
  // das ist kein hack... --boni
  if(GetController(pObj) == NO_OWNER && GetController(pObj2) == NO_OWNER)
    return(1);

  return(0);
}