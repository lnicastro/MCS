#define PTR INTEGER*8


      INTEGER*8 FUNCTION ipa()
      ipa = 1234567890
      RETURN
      END



      PROGRAM prova
      PARAMETER (IPTRW=2)
      INTEGER*8 i1
      INTEGER*4 i2(IPTRW)
      PTR i

      i1 = ipa()
      WRITE(*,*) i1

      i2 = ipa()
      WRITE(*,*) i2
      END
