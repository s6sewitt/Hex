#ifndef HEXLOCATION_H
#define HEXLOCATION_H

class HexLocation{
public:
   int x;
   int y;
   HexLocation(){x=0;y=0;}
   HexLocation(int ix,int iy){x=ix;y=iy;}
   HexLocation(const HexLocation& h){x=h.x;y=h.y;}
   ~HexLocation() { }
   bool operator == (const HexLocation& p) const { return ((p.x==x)&&(p.y==y));}
   bool operator != (const HexLocation& p) const { return ((p.x!=x)||(p.y!=y));}
   bool operator < (const HexLocation& p) const { 
      if (x != p.x) return (x<p.x);
      return (y<p.y);
   }
   HexLocation& operator =(const HexLocation& h) { x=h.x;y=h.y; return *this;}
};

#endif
