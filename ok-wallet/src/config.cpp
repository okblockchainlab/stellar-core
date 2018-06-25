#include <sstream>
#include <regex>
#include "main/Config.h"

namespace stellar {
void setNoListen(Config& cfg);
}

static std::string test_config("\
NETWORK_PASSPHRASE=\"Test SDF Network ; September 2015\"\n\
\n\
KNOWN_PEERS=[\n\
\"core-testnet1.stellar.org\",\n\
\"core-testnet2.stellar.org\",\n\
\"core-testnet3.stellar.org\"]\n\
\n\
DATABASE=\"sqlite3://<DATA_DIR>/stellar-test.db\"\n\
\n\
BUCKET_DIR_PATH=\"<DATA_DIR>/buckets-test\"\n\
\n\
UNSAFE_QUORUM=false\n\
\n\
FAILURE_SAFETY=1\n\
\n\
LOG_FILE_PATH=\"stellar-core-test.log\"\n\
\n\
\n\
#The public keys of the Stellar testnet servers\n\
[QUORUM_SET]\n\
THRESHOLD_PERCENT=51 # rounded up -> 2 nodes out of 3\n\
VALIDATORS=[\n\
\"GDKXE2OZMJIPOSLNA6N6F2BVCI3O777I2OOC4BV7VOYUEHYX7RTRYA7Y  sdf1\",\n\
\"GCUCJTIYXSOXKBSNFGNFWW5MUQ54HKRPGJUTQFJ5RQXZXNOLNXYDHRAP  sdf2\",\n\
\"GC2V2EFSXN6SQTWVYA5EPJPBWWIMSD2XQNKUOHGEKB535AQE2I6IXV2Z  sdf3\"]\n\
\n\
\n\
#The history store of the Stellar testnet\n\
[HISTORY.h1]\n\
get=\"curl -sf http://s3-eu-west-1.amazonaws.com/history.stellar.org/prd/core-testnet/core_testnet_001/{0} -o {1}\"\n\
\n\
[HISTORY.h2]\n\
get=\"curl -sf http://s3-eu-west-1.amazonaws.com/history.stellar.org/prd/core-testnet/core_testnet_002/{0} -o {1}\"\n\
\n\
[HISTORY.h3]\n\
get=\"curl -sf http://s3-eu-west-1.amazonaws.com/history.stellar.org/prd/core-testnet/core_testnet_003/{0} -o {1}\"\
");

static std::string main_config("\
LOG_FILE_PATH=\"stellar-core.log\"\n\
BUCKET_DIR_PATH=\"<DATA_DIR>/buckets\"\n\
DATABASE=\"sqlite3://<DATA_DIR>/stellar.db\"\n\
\n\
NETWORK_PASSPHRASE=\"Public Global Stellar Network ; September 2015\"\n\
\n\
# PREFERRED_PEERS_ONLY (boolean) default is false\n\
# When set to true, this peer will only connect to PREFERRED_PEERS and will\n\
# only accept connections from PREFERRED_PEERS or PREFERRED_PEER_KEYS\n\
PREFERRED_PEERS_ONLY=false\n\
\n\
# KNOWN_PEERS (list of strings) default is empty\n\
# These are IP:port strings that this server will add to its DB of peers.\n\
# It will try to connect to these when it is below TARGET_PEER_CONNECTIONS.\n\
KNOWN_PEERS=[\n\
\"core-testnet1.stellar.org\",\n\
\"core-testnet2.stellar.org\",\n\
\"core-testnet3.stellar.org\"]\n\
\n\
# NODE_IS_VALIDATOR (boolean) default false.\n\
# Only nodes that want to participate in SCP should set NODE_IS_VALIDATOR=true.\n\
# Most instances should operate in observer mode with NODE_IS_VALIDATOR=false.\n\
# See QUORUM_SET below.\n\
NODE_IS_VALIDATOR=false\n\
\n\
FAILURE_SAFETY=1\n\
\n\
UNSAFE_QUORUM=false\n\
\n\
CATCHUP_COMPLETE=false\n\
\n\
CATCHUP_RECENT=1024\n\
\n\
NODE_SEED=\"SBI3CZU7XZEWVXU7OZLW5MMUQAP334JFOPXSLTPOH43IRTEQ2QYXU5RG self\"\n\
\n\
#####################\n\
##  Tables must come at the end. (TOML you are almost perfect!)\n\
\n\
#[HISTORY.local]\n\
#get=\"cp /tmp/stellar-core/history/vs/{0} {1}\"\n\
#put=\"cp {0} /tmp/stellar-core/history/vs/{1}\"\n\
#mkdir=\"mkdir -p /tmp/stellar-core/history/vs/{0}\"\n\
\n\
# other examples:\n\
[HISTORY.stellar]\n\
get=\"curl http://history.stellar.org/{0} -o {1}\"\n\
put=\"aws s3 cp {0} s3://history.stellar.org/{1}\"\n\
\n\
# [HISTORY.backup]\n\
#get=\"curl http://backupstore.blob.core.windows.net/backupstore/{0} -o {1}\"\n\
#put=\"azure storage blob upload {0} backupstore {1}\"\n\
\n\
[QUORUM_SET]\n\
THRESHOLD_PERCENT=66\n\
VALIDATORS=[\n\
    \"GDQWITFJLZ5HT6JCOXYEVV5VFD6FTLAKJAUDKHAV3HKYGVJWA2DPYSQV  A_from_above\",\n\
    \"GANLKVE4WOTE75MJS6FQ73CL65TSPYYMFZKC4VDEZ45LGQRCATGAIGIA  B_from_above\",\n\
    \"GDV46EIEF57TDL4W27UFDAUVPDDCKJNVBYB3WIV2WYUYUG753FCFU6EJ  C_from_above\"\n\
]\n\
\n\
[QUORUM_SET.1]\n\
THRESHOLD_PERCENT=67\n\
VALIDATORS=[\n\
    \"$self\", # 'D' from above is this node\n\
    \"GDXJAZZJ3H5MJGR6PDQX3JHRREAVYNCVM7FJYGLZJKEHQV2ZXEUO5SX2 E_from_above\",\n\
    \"GB6GK3WWTZYY2JXWM6C5LRKLQ2X7INQ7IYTSECCG3SMZFYOZNEZR4SO5 F_from_above\"\n\
]\n\
\n\
[QUORUM_SET.2]\n\
THRESHOLD_PERCENT=100\n\
VALIDATORS=[\n\
    \"GCTAIXWDDBM3HBDHGSAOLY223QZHPS2EDROF7YUBB3GNYXLOCPV5PXUK G_from_above\",\n\
    \"GCJ6UBAOXNQFN3HGLCVQBWGEZO6IABSMNE2OCQC4FJAZXJA5AIE7WSPW H_from_above\"\n\
]\n\
\n\
[QUORUM_SET.2.1]\n\
THRESHOLD_PERCENT=50\n\
VALIDATORS=[\n\
    \"GC4X65TQJVI3OWAS4DTA2EN2VNZ5ZRJD646H5WKEJHO5ZHURDRAX2OTH I_from_above\",\n\
    \"GAXSWUO4RBELRQT5WMDLIKTRIKC722GGXX2GIGEYQZDQDLOTINQ4DX6F J_from_above\",\n\
    \"GAWOEMG7DQDWHCFDTPJEBYWRKUUZTX2M2HLMNABM42G7C7IAPU54GL6X K_from_above\",\n\
    \"GDZAJNUUDJFKTZX3YWZSOAS4S4NGCJ5RQAY7JPYBG5CUFL3JZ5C3ECOH L_from_above\"\n\
]\n\
");

void loadConfig(const std::string& net_type, const char* data_dir, stellar::Config& cfg)
{
  std::string config_str;
  std::regex datadir_re("<DATA_DIR>");

  if ("main" == net_type) {
    config_str = std::regex_replace(main_config, datadir_re, data_dir);
  }
  else if ("testnet" == net_type) {
    config_str = std::regex_replace(test_config, datadir_re, data_dir);
  }
  else {
    assert("unknown net type." && false);
    return;
  }

  std::istringstream cfg_stream(config_str);
  cfg.load("-", cfg_stream);
  cfg.NTP_SERVER.clear();
  stellar::setNoListen(cfg);
}
