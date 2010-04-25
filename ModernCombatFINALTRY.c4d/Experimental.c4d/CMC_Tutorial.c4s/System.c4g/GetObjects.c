#strict

global func GetObjects(id ID)
{
  var obj, exp = Find_And(Find_Not(Find_Func("IsHUD")), Find_Not(Find_Func("IsLight")),Find_Not(Find_Category(C4D_Goal)),Find_Not(Find_Category(C4D_Rule)),Find_Not(Find_Category(64)));
  if(ID)
    exp = Find_And(Find_ID(ID),Find_Not(Find_Func("IsLight")),Find_Not(Find_Func("IsHUD")));
  var extra, extray;
  for(obj in FindObjects(exp))
  {
    extra = "";
    extray = 0;
    if(obj->GetR() != 0) extra = Format("->SetR(%d)", obj->GetR());
    if(obj->GetCon() != 100)
    {
      extra = Format("->SetCon(%d)", obj->GetCon());
      extray = -GetDefCoreVal("Offset", "DefCore", GetID(obj), 1) * (100-obj->GetCon()) / 100;
    }
    if(obj->GetID() == LADR || obj->GetID() == CRN1)
    {
      extra = Format("->Set(%d)", obj->LocalN("length"));
      if(obj->GetID() == LADR)
        extray = - (obj->LocalN("length")-1) * obj->LocalN("sizeY");
    }
    if(obj->~LastIndex()) extra = Format("->SetRail(%s)", iArray2String(obj->LocalN("parttype")));
    Log ("  CreateObject (%i, %d, %d, %d)%s;",
       GetID (obj),
       GetX(obj),
       GetY (obj) - GetDefCoreVal("Offset", "DefCore", GetID(obj), 1) - extray,
       GetOwner (obj),
       extra);
  }
}

global func iArray2String(aArray)
{
  var str = "[", i;
  for(var a in aArray)
  {
    str = Format("%s%d", str, a);
    i++;
    if(i < GetLength(aArray)) str = Format("%s,", str);
  }
  str = Format("%s]", str);
  return(str);
}
