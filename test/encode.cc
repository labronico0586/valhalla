#include "valhalla/midgard/util.h"
#include "valhalla/midgard/pointll.h"
#include "test.h"

#include <string>

using namespace std;
using namespace valhalla::midgard;

namespace {

using container_t = std::vector<std::pair<double,double> >;

bool appx_equal(const container_t& a, const container_t& b) {
  if(a.size() != b.size())
    return false;
  for(size_t i = 0; i < a.size(); ++i) {
    if(!equal(a[i].first, b[i].first) || !equal(a[i].second, b[i].second))
      return false;
  }
  return true;
}

//need ostream operators for some of these types
std::string to_string(const container_t& points) {
  std::string out = "{";
  for(const auto& p : points) {
    out += "{" + std::to_string(p.first) + ", " + std::to_string(p.second) + "}";
  }
  out += "}";
  if(out.length() > 2)
    out.erase(out.end() - 3, out.end() - 1);
  return out;
}

void do_pair(const container_t& points, const std::string& encoded) {
  auto enc_answer = encode<container_t>(points);
  if(enc_answer != encoded)
    throw std::runtime_error("Simple polyline encoding failed. Expected: " + encoded + " Got: " + enc_answer);
  auto dec_answer = decode<container_t>(encoded);
  if(!appx_equal(dec_answer, points))
    throw std::runtime_error("Simple polyline decoding failed. Expected: " + to_string(points) + " Got: " + to_string(dec_answer));
  //cant run this thorough of a test due to accumulation of error
  /*if(encode<container_t>(decode<container_t>(encoded)) != encoded)
    throw std::runtime_error("Nested polyline encoding of a decoding failed");
  if(appx_equal(decode<container_t>(encode<container_t>(points)), points))
    throw std::runtime_error("Nested polyline decoding of an encoding failed");*/
}

void TestSimple() {
  /**
   * #to generate these a new test cases grab google polyline encoder for python:
   * wget "https://py-gpolyencode.googlecode.com/svn/trunk/python/gpolyencode.py"
   * sed -i -e "s/15E/16E/g" gpolyencode.py
   * #then generate a test case with python:
   * python -c "import random; import gpolyencode; x = [ [round(random.random() * 180 - 90, 5), round(random.random() * 360 - 180, 5)] for a in range(0, random.randint(1,100)) ]; p = gpolyencode.GPolyEncoder().encode(x)['points']; print; print 'do_pair(' + str(x).replace('[','{').replace(']','}') + ', \"' + repr(p)[1:-1] + '\");'; print"
   */

  //check an easy case first just to be sure its working
  auto encoded = encode<container_t>({{-76.3002, 40.0433}, {-76.3036, 40.043}});
  if(encoded != "gq`kkAny~opCvQnsE")
    throw std::runtime_error("Expected: gq`kkAny~opCvQnsE but got: " + encoded);

  //note we are testing with higher precision to avoid truncation/roundoff errors just to make the test cases easier to generate
  do_pair({{41.37084, -5.03016}, {76.8342, 42.01251}}, "~o_rHola|mA{egvxA_kosbA");
  do_pair({{-86.36737, 90.75251}, {22.62106, 29.07404}, {-29.06206, -163.63365}, {-37.89193, 2.07912}, {-21.17342, -109.54591}, {21.73208, 2.41464}, {6.16554, -156.67554}, {-29.67467, -69.32163}, {-11.72187, -43.68974}, {69.87996, -66.81097}, {65.1444, 122.14861}, {55.37597, -72.80306}, {46.72793, 61.50523}, {53.03528, -0.71103}, {-59.28544, -148.6204}, {24.87703, -81.46813}, {41.67281, -174.44407}, {-75.46263, 168.7024}, {0.5092, -100.36431}, {80.44686, -135.06572}, {41.94477, 20.19688}, {-26.1842, 49.92658}, {-37.94516, 96.65621}, {-81.25373, 43.06368}, {2.17062, -58.23909}, {-24.95552, -100.32671}, {-60.27096, -44.25241}, {-71.96091, -162.75252}, {57.27244, -37.0602}}, "{~ablDrgmvcDjwpstB{_c{nEre}pnJ~qnqaBcwha{Hzy|yOjs`|sE{il{^k}optEwxvypAf_bmnHvkbu\\kbtreDblojcAchm{o@_awfa@zsebk@kqqszCwtdlgJnc``Hj~{yrJz}esQc{od_GnuynOfcktuBk`~_KrntbyG~kofuE{wsa_Ckyzo_DfsxipDggc`_@k}~nmS~ckl~Ej|oe`Okn}{oCbe_eaAwf_nwCogncgHrk~lhAgypuw@rkg}`C{xccxA~rylUb|_feBr`jrqAb~_f`E{dyb~CfkygoAvvsvr@wbo}iB~mnjbAzot_aFz|nhU_ysvnFk{wnuF");
  do_pair({{-15.14467, 95.09918}, {-0.52122, -136.82914}, {65.10222, 77.44049}, {-69.52769, -130.85075}, {46.36195, -33.8091}, {4.62432, -55.24198}, {-85.23295, 56.73264}, {-34.87542, -148.58007}, {-88.75693, 73.15576}, {14.66023, -174.40911}, {-17.84068, 133.05384}, {74.64337, -148.80224}, {-61.31799, -145.47643}, {45.99419, 133.9817}, {34.77185, -75.40288}, {35.42642, -167.09832}, {84.2264, -103.19799}}, "wqkktDzlj{[~~wjyLslp{Z{j~twK_xid|BnyahlKj`dx_Gcn}axDo}i`|E~ad{g@zxmrnAwlkqtEjnmkjDjkhrfKspq`_Bkxt|eLjitweBjcdewMosagdEkq`miQziu~|@~ccrxOc|wkoDcv~iE~olibGatw_tOgbytkEdgyjnKfs}kT~ss{mDsm}f@sid{xBwmoa|A");
  do_pair({{10.33681, 13.20223}, {-54.25453, 52.78416}, {2.80146, -1.14462}, {-75.1119, 34.72812}, {-36.89225, -6.75353}, {-79.98349, 51.41082}, {-46.16035, -36.24582}, {12.0213, 165.51765}, {23.77675, -125.52299}, {64.00935, -107.47264}, {61.64121, 17.30004}, {64.06288, 59.41232}, {-5.53452, -16.93113}, {-62.60699, -124.69433}, {-2.21207, 150.86091}, {-12.06676, -65.27085}, {88.81366, -126.01352}, {37.10442, 60.08433}}, "krxdXsa|uRsm{njAv}iezBvspzeBknlykBg}nlcA~omrsCbzybnAcwv{gA{fa}mBnqaeqA~jcefDgyko_A{gty_Kc`c~mB~vubjPsznlU{qula@ohrvkAoso~lFvgpoCopiioAkyxrCrhsrpCnl{vcC~~iplEjtlzkBoypqlOoyeerB~isf{KbmnxQz_mzrBgagl_Esjo}aJnrasaB");
  do_pair({{80.80723, -16.19035}, {-7.92885, 14.12617}, {-62.89544, -83.80016}, {-3.75161, -43.61774}, {-12.55465, -144.02106}, {32.61274, -168.99683}, {-52.80244, 82.31771}, {43.11487, -27.98147}, {21.44349, 102.0805}, {5.8727, -135.69082}, {42.42274, -4.14301}, {57.51329, -174.30708}, {-70.5744, 126.85297}, {3.48474, 59.47075}, {16.26716, -38.793}, {65.67464, 55.57602}, {78.45655, 65.85946}, {-70.17561, 169.4693}, {-48.9856, -65.40795}, {44.18882, 15.81999}, {-75.78086, -168.70654}, {70.58843, 0.90691}, {58.08244, 25.21063}}, "zwd{]{racyCo}jyx@~s_ghDrj}wyDzr{ygBghpskAkhzxoBnfco~D~lhxOrxksn@{pxcuAwu_j~Mvoi|aDvackqE{ni}uDchjawFf|uih@nblodMjuju\\ai`|yFoxyudAhu~pcIkv`x[cfjl}PrgzhsFv{to_CgweglCjkplzDgtdkWwny~rDo~qf}A_zsrRktckW_~yrdE~dxnzGbsw~~Lsvilg@giw|yCgh|upDbkr}~I~wjycFsgkobIsotdvGoekjm@jwhzV");
  do_pair({{-66.08387, -49.71979}, {-73.78285, -48.51323}, {9.34045, 113.87993}, {43.50452, 1.63821}, {-34.9178, -80.0957}, {10.05031, 156.94869}, {-1.01914, 42.48849}, {-60.53017, 29.84393}, {-80.30977, -63.88584}, {55.21413, -50.18415}}, "z}sy}A|`m`}B_qshAda|tMocuvtHgemp}CnbuauEkued`Ajps{zC~qoqtCki`ccM{iswtAnabiyEr_sbT~kwbWjngopBr}xwqD~_gvd@sbhcYwbvnaG");
  do_pair({{-9.59238, -59.68037}, {-80.40085, -45.96192}, {-50.2141, -91.43616}, {22.94273, -10.23652}, {89.83107, -95.51344}, {-53.17299, 125.8809}, {-71.34097, 3.57291}, {-11.00935, -13.60961}, {-18.53575, 21.3904}, {42.63423, 12.33067}, {-25.3997, -49.97198}, {30.27697, -11.32676}, {80.92531, -171.47488}}, "bfrypBvbnhQczhdYjxx`fC~zovuA{nmqx@o``{yC{lcpjCnn{saDghpq~BgyygeLvxgwoGjxahhFvi{sa@nrvw_@gejarBskfwaA~~jjMbx}gPwrotsBrzsyuBrwmw`Cg}uuhA{nfeiBnhtmpHgpir_B");
  do_pair({{30.4013, 100.74702}, {-50.40126, -89.19032}, {-69.43379, -124.72317}, {88.81924, -4.02525}, {-70.20336, -63.86226}, {28.45957, 76.98844}, {-50.271, -2.71977}, {-22.10004, 25.22313}, {-11.80191, 179.3426}, {-3.68762, -38.43544}, {-24.49249, 71.05112}, {-7.64003, -131.0033}, {-31.71518, 162.92441}, {-11.89722, -26.58333}, {-54.43309, -148.41607}, {54.44587, -104.34535}, {87.49116, -99.99561}, {-74.03822, -161.68331}, {-16.70451, 112.07603}, {16.28222, 114.26197}, {60.96175, -130.56696}, {53.15759, -124.62571}, {13.86561, 136.02146}, {-39.9363, 161.60008}, {-59.07013, 62.61662}, {85.62251, -145.65735}, {-14.96798, 166.5488}}, "wobd_Egpp~x@vjzgiJ~nxbyCbbwwbAb|shc@_cyeeFk}~ylHbddcqBn_~hnHw_zskGc`|d{Dbb_`wCrsiduCg~nht@_llvt@{qu}dHcppsRnvbk~KcfgnN_}oyoEjoytf@fplk`Kw}qc_@{p|roPzwl|l@vhsmhJo}qxd@fqakgFzzdcpA_sz`sA_fmtnEwqnhGsq|_~@fxbttBfa~asHwe}ciO{{jjlBglldCqej|}@bod~qMuu_ftAc_siJ~ni{McoscpNvce}iAwfexo@jzxreBfanx{Djwync@bb`glK_yi~rGkspnrQrxpz~D");
  do_pair({{23.86009, -92.13773}, {88.11646, -35.54743}, {-10.58013, -128.94492}, {-83.30518, 110.36499}, {-85.0673, 8.89085}, {63.74281, -159.81506}, {20.75902, 32.05697}, {-66.18106, -49.9208}, {-60.21468, -124.85972}, {48.87687, -33.15284}, {62.43557, 118.97874}, {-49.30944, -65.14801}, {67.52368, -16.90717}, {37.65532, -5.85065}, {41.84569, -158.57244}, {-88.44961, 49.85075}, {-78.15121, 8.73399}, {47.30357, 149.61304}, {39.52699, 22.91384}, {22.24144, -62.60971}, {-13.93741, 178.84024}, {-56.20882, -109.4818}}, "bosvnDsvhol@w|~|jBcv{pyBb~ocqDzw}f{DkdimgMrbxuiCvtnp`EnspjBjv~w`I{vsyzG{`}}lJzuo~pAryoj{C~ykydDna||mCwadkJ__j|mD{llaoEw~jdaHwypzXzpee~Ib~jctEokk_{A_u|y}EowyaTns_~w@zvkhbHciw~FkpcplKf_rowFndqlmA_aqsRskqukGwzchnF~jbtpFfssyMzd}bbDzy_~_@{||okMbyd_dAn~w|dPbj`soA");
  do_pair({{-7.52569, 68.21456}, {-61.33866, 152.53755}, {53.8972, 16.75419}, {64.55989, -111.07589}, {53.25229, -45.80257}, {65.04029, 59.64022}, {59.504, -47.47756}, {5.20109, 130.25033}, {65.99088, -54.66988}, {-0.25929, 89.63035}, {-85.36721, -139.15146}, {-22.17328, -137.62681}, {-74.94886, -72.13421}, {-32.88714, 153.28987}, {21.31903, 11.5894}, {38.21212, -8.08468}, {82.36711, -79.93899}, {-16.43777, -16.22507}, {-4.6856, 151.33454}, {43.6428, 38.2291}, {36.94968, -165.55867}}, "_qnbaCrrijM{ity_DrmnseB~zp~aGg`mxzE~bcyrFaqxiSo}}n{B|cdqTk}ubhE_mnnUfl}hkEba|pIaxs~qIzjkqfB`hsu_J{`i}rBkkkfrGtzqj}BbnvjsL|kqiaDsy`|Ass`pwBofj|{BvhdtcB_lx}lLmxffoAjvugmGulnkfB`}xod@ciaf_@hms`hC{e_fsA_gxoxB~gqm{Dsz_r~HsmhlU~`lvvE_lvd{Ar~eucK~nowK");
  do_pair({{-34.28558, 167.99824}, {-7.31514, 10.55876}, {-79.67137, 157.13177}, {25.71649, 109.01104}, {-5.60888, -38.33339}, {-18.47462, -162.75213}, {88.94416, -102.91984}, {73.00401, 133.35917}, {-23.42671, 43.84914}, {-9.0552, -173.10419}, {-83.59144, -49.70882}, {86.19484, -62.12698}, {1.39314, 87.82391}, {-22.75176, 74.65006}, {-23.76837, -112.30955}, {-84.46087, 109.49098}, {-40.22217, -177.55086}, {-5.12028, -96.81104}, {-44.24017, 64.98831}, {-89.65149, -132.13723}}, "_qxl_Iv_sk`An~ihkHoscmr@clbqvGjwg_iCrx`xzAgtj_hEzae`xGrj}vz@fj|hlFvkgpWc}zbqBwhi{kEcmitaMjb|k]zggviD~ft|vDb~wx|KkjdlZsqmjjF~qidmC~e}tVoy|ybIsyg_}Gfi{v`DrdacXf_u`m@rnbrcJbq`}@cds`fLf`kwrB~qsnbPwubksAw}}~xCckm}aAkjmrsHb`uriAfyq~vJn~truA");
  do_pair({{-17.83241, 74.80488}, {82.06795, -87.62972}, {52.4185, -100.10177}, {60.88432, 177.62815}, {76.50145, -11.83659}, {-9.25737, -133.52694}, {-87.18693, 9.53612}, {73.29193, -158.30913}, {80.11725, 106.80563}, {-61.33643, -39.91792}, {-22.29452, -166.41778}, {83.73418, 162.48267}, {12.59667, 25.84381}, {-22.74831, -128.22072}, {76.66833, 39.9599}, {-58.66834, -19.87026}, {11.38746, -164.77395}, {41.83013, 53.10491}, {71.32073, 107.45972}, {-2.04772, 93.57824}, {45.00988, 160.76223}, {88.12149, 18.29801}, {13.22053, 110.10683}}, "_hvtmCr|k_a@nafytHo{lp}DbnfxVratpw@_ghvpOwxucOfi_khJseex\\zmkbgFfuhqbDg`{zoGndmssCbwmc_IwgzaqHogitxNouq_LzlhzvG~usxlGf`}gpFkv|miAcunirRwhnfiEvwwrcGjm{tfCbhjzdHfdhlbAww|w_I_c|r|D~wvbqBzlhcaGr_fksGovzrdCwsgq~K{iaay@svptfBoy}fw@nggnYbw`}jCkvqc_C_kdwxAvlivnGsjifqAgbqbnD~|qzmC");
  do_pair({{-86.21289, 102.32069}, {68.55528, 120.35663}, {87.02457, -101.81795}, {52.51881, 150.6204}, {86.63575, -147.25056}, {81.72213, 54.03893}, {75.25834, 172.67519}, {48.51055, -123.77748}, {-83.26863, -166.50616}, {-88.44121, 119.62017}, {16.34476, -142.32344}, {-29.09806, 57.11145}, {3.71511, -90.46076}, {-10.04222, -25.46628}, {11.75672, -62.30591}, {33.56227, -130.45443}, {16.91946, 13.04033}, {0.82637, 147.98325}, {-13.01717, 33.403}, {6.83347, 20.22308}, {69.23392, 159.75933}, {-12.21637, 10.90932}, {-75.80103, -174.46565}}, "cbddbErp_mcDgmyka@sqhefHfvmwfLsygfb@{srn`N~`ay`A~ercwPwsia`Acpv|~Jf|{jHgm~gaFzqoiKzb`mtPzwp_r@nm}npAvedjzFsnwv`PfuuzHrrwrrNcbszfEsgpk{JfortuAb~anxGcfwq}@_j}}zBbxtfYzkogeAw`oqh@nqm~`C{}{qh@oefupGrnxv^okgk`Gbyfu]rplpyEfd}kY~_mcX_xqzd@svrciGc{r_vBrta|zGbrijzCrnkq`Jfx{gxB");
  do_pair({{82.55686, 40.18149}, {-79.6624, -167.21023}, {-1.04267, -66.55423}, {61.73513, -170.40375}, {9.68077, 110.51218}, {79.7428, 104.78995}, {78.90557, -78.36458}, {-0.7791, -149.20926}, {-67.01631, -126.5595}, {-82.51481, -19.16425}, {53.91056, 7.59478}}, "cnnskAwzzm|Cn}dqjKvnaltH}vp~~Dcdq}tC|anaeEo{svvBs|vxvOntchbBjfg|I{{fsdCbuzi|Izebr@no_cfCzbq~vC_ylej@tpxi}Bcj{ykEdo}p\\kvf`r@sxvecG");
  do_pair({{0.23265, 88.237}, {71.47586, -61.6458}, {-75.11235, 14.23395}, {-24.01342, 20.11683}, {1.34031, -165.05875}, {-39.91151, -167.83088}, {-10.62305, -48.34365}, {2.63151, 34.26806}, {-56.37729, -103.01014}, {77.59691, -84.12264}, {51.40339, -153.14642}, {-84.59431, 46.03929}, {86.29481, 70.61137}, {50.92146, -145.15046}, {-8.92886, -102.84905}, {13.35157, -20.37454}, {24.0645, 69.84886}, {-2.5586, 5.40286}, {37.03896, 27.7622}}, "o{phgDskeM~yb{|Gs{i{fCksivoCbb`rvG__afJcrym`Bv`fe`Jc_njo@biehDv}xtmA{n|{bFw_szv@{~eq|C_y~gXnnxydG~prpoBwsx_c@ojcp~Ffa{sbC~ev}p@{qi|zJfoskbG_jwzm@_yf}dIjy`pzKjq_nbAc}ztoA~c~cqB{_zh|C{m{ni@oqxakDcuzlS~an|yBvf}wq@wqusi@o~yojA");
  do_pair({{-3.37496, 150.44572}, {34.04592, -29.5979}, {-30.31843, 30.76149}, {15.28236, 50.02759}, {16.8507, -51.85793}, {56.52462, -138.59662}, {-15.07377, 179.03104}, {-48.6195, 171.91062}, {-68.61757, 34.85549}, {-88.33354, -36.48368}, {60.37303, -171.00903}, {22.18385, -54.91201}, {-33.7173, -91.77015}, {76.7102, -149.66336}, {74.556, -11.73522}, {72.26764, -114.08019}}, "ohn}}G~u~lEfu~kvI_t~jfA{l`crBzrnwyBgb|vc@k`g~uA~criaEgdv~AbobmdD_cotjAwomy|Qjr_qgCfarqLb_n~~@rxdldGjvqce@bieagCbwjrd@jywq_GsoiszGw~~l|Evf{ygAvpsheAz||riBrtolmBwu}rqEw{mafGnlnbCrosebEnmtjC");
  do_pair({{4.42507, -71.53103}, {58.88511, 91.12619}, {-47.40182, -61.57476}, {-8.03374, 46.32079}, {-45.46749, 51.64678}, {60.72559, -11.43758}, {-24.29685, -107.12714}, {46.4687, -40.18084}, {-24.33475, 23.64071}}, "j`|lgC{uamGg{xfuHog~zfBj`cgbHbtfviE{nlxlE_xyajAkiadIjxwkfAnsjiwBojopiEntlouDnmjdaDwnau~B{}d~eC{mjvxBr~n`fC");
  do_pair({{-41.91668, -13.44214}, {57.60493, 138.65963}, {-0.53819, 78.64398}, {-86.49416, 62.6639}, {86.50376, 120.06918}, {-39.41316, -5.40548}, {62.98101, 31.26041}, {-73.0198, -63.80468}, {1.68253, -9.39797}, {66.77633, 74.13591}, {-25.38422, 41.77865}, {-43.60725, -142.36685}, {-73.59996, 165.75833}, {68.17974, -91.1608}}, "vdmsXnok}nAswpbaHsciy|DbaanqB~ww{mB~ajn]bwi}bD_uvnlB_q}}hIfujinFnnjdoFca||dAsrshbEb_iitDrqykbGkbvwfBs~mnmCoboi~Coi_d{Bvw|u|@ja`xnDvdjf~Ijzfwa@w~mujQjorex@rdb`iNgnplmG");
  do_pair({{-75.58665, -146.4987}, {84.40365, -124.06332}, {55.91544, -16.16103}, {-78.63185, -70.22471}, {11.07045, -93.40103}, {40.59153, -120.5687}, {32.65514, -78.45088}, {-82.43402, -100.86241}, {66.80685, -141.99054}, {6.82737, -62.36202}, {47.45839, 94.1812}, {12.19556, -1.73659}}, "vgqlvGrdmdoCgbjxi@w``dpHctyxlEb`xiu@~bxbfBrtbs_G~fqek@wx~ajDjzdyr@ojyhw@wjtioAjwkcNro{vi@nonozEbkgmmAka}s{Gomc{vCnlzkqBgnsqiHwm|nlAzlj}uDzuggbA");
  do_pair({{84.12126, 72.27868}, {-19.91672, -174.43161}, {-22.55542, 167.45326}, {-38.5977, -19.98754}, {2.54066, -165.79124}, {-65.02688, -84.94664}, {-68.08338, -64.17942}, {44.13348, 43.02832}, {-0.88703, -151.66338}, {-76.50135, -101.43089}, {-7.62893, -99.24727}, {79.11923, -30.83726}, {-76.93034, 2.94778}, {-7.1857, -75.70398}, {-10.9239, -10.6345}, {-29.18119, 5.47514}, {26.84891, 75.47435}, {-7.04824, -37.96123}, {-14.68048, -156.22143}, {59.84613, -167.75286}, {-88.33815, 125.94346}, {71.80321, -81.51938}, {-89.91225, 141.35892}, {-15.68731, -146.83647}, {-54.43659, -28.82345}, {-71.59295, 7.44686}, {24.65526, -68.24392}}, "ohpzhCwijm_Db`_quMvd~leEk{~akSvu``D~hnodJnqcr]fzcbuGoj{mmAorjeyChz~z_Cg~orf@dvpyDwbmnkEwpd`uEff`jrJztyztAsi}x~A~ecfoCg{gdCgmsjbCshlnaC__umdD_lam_AbioshH~uo_uC__{_dCoyob{BnddcFocgv]rwiya@selodCgxxziBvzpjwEzj|s_An}_q`F~uypMjiy~TcxvcmC_sxdoPnlmsyGnzoujK_bgmpHwdlbhLfoimsHzr`udPwqiqlCwd}a`F~da|hAkmwddAnocv_@vpxjoCcxoqvD");
  do_pair({{-9.42372, 152.03805}, {-1.82375, 116.05687}, {-71.41203, -66.66489}, {65.64729, 68.17239}, {34.2284, -77.90916}, {-72.90402, -47.25247}, {-78.55439, -25.28158}, {-31.92992, 103.20477}, {58.26482, -169.02219}}, "cit~`Hnud~PvvbscAcuznM~lmo{InrivcC_ayd`Go~lldGzobsuGr_t|z@cjcny@f_zikEs{~{h@b{zwI{{datFklv|wA~glffOgr``kD");
}

}

int main() {
  test::suite suite("encode_decode");

  // Test kilometer per degree longitude at a specified latitude
  suite.test(TEST_CASE(TestSimple));

  return suite.tear_down();
}
