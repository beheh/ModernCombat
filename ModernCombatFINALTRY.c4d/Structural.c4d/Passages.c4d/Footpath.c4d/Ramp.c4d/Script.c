/*-- Rampe --*/

#strict 2

local x,y,mat;


/* Initialisierung */

protected func Initialize()
{
  SetObjectBlitMode (GFX_BLIT_Additive);
  Set(30,-10,"Rock");
}

public func Set(int iX, int iY, string szMaterial)
{
  x = iX;
  y = iY;
  if(szMaterial)
    mat = szMaterial;
  Update();
}

public func Draw()
{
  Log("DrawMaterialQuad(\"%s\",%d,%d,%d,%d,%d,%d,%d,%d,true);",mat,GetX()  ,GetY()  ,
                                                                   GetX()  ,GetY()+y,
                                                                   GetX()+x,GetY()  ,
                                                                   GetX()  ,GetY()  );
                                                                   
  DrawMaterialQuad (mat, GetX()  , GetY()  ,
                         GetX()  , GetY()+y,
                         GetX()+x, GetY()  ,
                         GetX()  , GetY()  , true);

  Sound("Connect",true);
}



protected func Update()
{
  var x1 = 0;
  var y1 = 0;
  
  var x2 = x;
  var y2 = y;
  
  if(x < 0)
  {
    x1 = x;
    x2 = -x;
  }
  
  if(y < 0)
  {
    y1 = y;
    y2 = -y;
  }
  
  SetShape(x1,y1,x2,y2);
  
  SetObjDrawTransform(x*100,0,0,0,y*100,0);
  
  var matnum = Material(mat);
  var rgb = RGB(GetMaterialColor(matnum,0,0), 
                GetMaterialColor(matnum,0,1), 
                GetMaterialColor(matnum,0,2)); 
                
  SetClrModulation (rgb);
}

protected func UpdateTransferZone()	{Update();}