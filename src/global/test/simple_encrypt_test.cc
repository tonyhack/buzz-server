//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-30 16:14:23.
// File name: simple_encrypt_test.cc
//
// Description:
// Define test of SimpleEncrypt.
//

#include <string.h>

#include <iostream>
#include <string>

#include "global/simple_encrypt.h"

static std::string test_string[12] = {
  "dskjlf8234jkljds78at43",
  "kl;dot439029485ifhdxmvejgiwqg",
  "=0fd8s90a4lkbnb3kl4j3jklgjas2316431*(^",
  "*(&^&#$KKGJ9893LKHG*#()*@%_(EJGLDPSG#GDFSGI#@",
  "()MKM<>>??{P(*Ujhjifeuiwuhrh329hfdlsj38r2",
  "}{sdfkmwefgkljwrj9025*)80j90j09jg490jd909JIOPJ_j(()uu*^&^%$^%^&*()bvhji",
  "%%^&*()DKJGIOUEOINHVDiopj3209gjsdj290uj))*#&IUVGDK",
  "(&*^^&(#*$_FDJVKJDFU)(E*R(#UDVJSEIJH@(#*RH*()EH@FG",
  "*(#Y%@HOLDKJKSNVIOWJF)#$@)&%*HDSKLGJSDKLJF#@()%&)*YWDIFGSDKLJF_#RJ(_pdjgiksfh83920u08",
  "*)*Y(i423j08vd8(H#LKds0v0293jrJKLDJLHFDSAY*(38yhrhfejhf*(&*#$jkdshf8932hflHDF*S(hdofh234fhsdf",
  "*(#HKLDJKSJF_G(W$JGKLDSJFGH$WT*(DWHSGKLJH#$FG)DWHGKLDSGH#@)JGDSKLGJ",
  "*()YHNUN(*Y*&GJHB()*U*(^&*GYHhoijgdf0sfu234hfkljdf083j)(JTIjh90342jtgpoj()$_U^T)IHG*($Y&GEW#*(H@O$TGHDIOW",
};

static const std::string kKeyString = "buzz-source-key";

int main() {
  size_t pos = 0;
  for(; pos < 12; ++pos) {
    char enc[512] = {0};
    int size = 512;

    global::SimpleEncrypt::Encrypt(test_string[pos].c_str(), kKeyString.c_str(), pos * 10, enc, size);

    char unenc[512] = {0};
    size = 512;

    global::SimpleEncrypt::Unencrypt(enc, kKeyString.c_str(), pos * 10, unenc, size);

    if(strncmp(unenc, test_string[pos].c_str(), size) == 0) {
      std::cout << "Pass " << pos << " test!" << std::endl;
    } else {
      std::cout << "Error in " << pos << " test!" << std::endl;
      return 0;
    }
  }

  return 0;
}

