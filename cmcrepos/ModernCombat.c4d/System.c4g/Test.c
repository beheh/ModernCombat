#strict
#appendto HZCK

public func Initialize()
{
  AddMsgBoardCmd ("Ausrüsten","Ausrosten(%s)",true);
  AddMsgBoardCmd ("FD","FakeMeDeath(%s)",true);
  return(_inherited());
}

global func FakeMeDeath(szPlr)
{
  var iPlr = GetPlayerByName(szPlr);  
  var clonk = GetCursor(iPlr);
  
  if(!IsFakeDeath(clonk))
    clonk->FakeDeath();
  else
    clonk->StopFakeDeath();
}

global func Ausrosten(szPlr)
{
  var iPlr = GetPlayerByName(szPlr);  
  var clonk = GetCursor(iPlr);
  clonk->CreateContents(PZFW)->DoAmmo(MIAM,1000);
  clonk->CreateContents(P29W)->DoAmmo(STAM,1000);
  clonk->CreateContents(FAPK);
  clonk->CreateContents(CDBT);
  clonk->CreateContents(NODG,0,4);
  if(FindDefinition(JB4K)) clonk->CreateContents(JB4K)->Activate(clonk);
  clonk->SetClrModulation(RGBa(0,0,1,200));
}

//*tss* *tss* Also echt Henry.