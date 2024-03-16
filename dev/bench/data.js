window.BENCHMARK_DATA = {
  "lastUpdate": 1710599410114,
  "repoUrl": "https://github.com/apytypes/apytypes",
  "entries": {
    "APyTypes Fixed-Point Array Benchmarks": [
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "703317eb1539f6e42d6eb162d4b17b4c1ee7ce9e",
          "message": "Upgrade pip and try to keep previous data",
          "timestamp": "2024-03-14T22:53:58+01:00",
          "tree_id": "fec2a0bf5349e26201cae3349bd27a60e3fe09d2",
          "url": "https://github.com/apytypes/apytypes/commit/703317eb1539f6e42d6eb162d4b17b4c1ee7ce9e"
        },
        "date": 1710453423738,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24940.67339352915,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015761666989121426",
            "extra": "mean: 40.09514836353415 usec\nrounds: 24157"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 113183.81834255722,
            "unit": "iter/sec",
            "range": "stddev: 6.987248338757874e-7",
            "extra": "mean: 8.835185229159203 usec\nrounds: 108838"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1217.926807243039,
            "unit": "iter/sec",
            "range": "stddev: 0.000025940494106710278",
            "extra": "mean: 821.0674024522465 usec\nrounds: 1225"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1035366.4081494488,
            "unit": "iter/sec",
            "range": "stddev: 6.269326223710309e-8",
            "extra": "mean: 965.8416499983913 nsec\nrounds: 103210"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17029.745173576255,
            "unit": "iter/sec",
            "range": "stddev: 0.0000024281802423835734",
            "extra": "mean: 58.720784709780794 usec\nrounds: 17005"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 185341.27520298102,
            "unit": "iter/sec",
            "range": "stddev: 3.958669893878424e-7",
            "extra": "mean: 5.395452248317735 usec\nrounds: 175439"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2043.1423011541376,
            "unit": "iter/sec",
            "range": "stddev: 0.000017473153261300007",
            "extra": "mean: 489.4421692679538 usec\nrounds: 2050"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 190459.66950892645,
            "unit": "iter/sec",
            "range": "stddev: 4.0787932563897705e-7",
            "extra": "mean: 5.250455398659254 usec\nrounds: 132732"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2341.8803299657084,
            "unit": "iter/sec",
            "range": "stddev: 0.00000659536235025423",
            "extra": "mean: 427.0073014425305 usec\nrounds: 2352"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 194549.48727320856,
            "unit": "iter/sec",
            "range": "stddev: 4.706289209648555e-7",
            "extra": "mean: 5.140080367293314 usec\nrounds: 177274"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2206.993847276767,
            "unit": "iter/sec",
            "range": "stddev: 0.0000190925738225806",
            "extra": "mean: 453.10502393738454 usec\nrounds: 2214"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 221155.39141300844,
            "unit": "iter/sec",
            "range": "stddev: 2.8664438352596135e-7",
            "extra": "mean: 4.521707536093916 usec\nrounds: 109446"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2483.5651273181497,
            "unit": "iter/sec",
            "range": "stddev: 0.00000678700008382706",
            "extra": "mean: 402.64698074571487 usec\nrounds: 2493"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10557.225164636759,
            "unit": "iter/sec",
            "range": "stddev: 0.000003107797844812452",
            "extra": "mean: 94.7218596179678 usec\nrounds: 10450"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 167.24399825770493,
            "unit": "iter/sec",
            "range": "stddev: 0.00006159659130290434",
            "extra": "mean: 5.979287809533877 msec\nrounds: 168"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "a8a6674187eedf9f9d8e7ed0a27e6ef9820553eb",
          "message": "Remove force_orphan",
          "timestamp": "2024-03-14T23:02:29+01:00",
          "tree_id": "016aaa7ed2fdba2d99a5c774b56599b550039fd8",
          "url": "https://github.com/apytypes/apytypes/commit/a8a6674187eedf9f9d8e7ed0a27e6ef9820553eb"
        },
        "date": 1710453929437,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25375.11559218476,
            "unit": "iter/sec",
            "range": "stddev: 0.0000012778586417994946",
            "extra": "mean: 39.40868747443217 usec\nrounds: 24670"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 113214.41568966307,
            "unit": "iter/sec",
            "range": "stddev: 5.308497966354984e-7",
            "extra": "mean: 8.832797430506934 usec\nrounds: 108496"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1219.2678321068317,
            "unit": "iter/sec",
            "range": "stddev: 0.000009965238532220896",
            "extra": "mean: 820.1643426219584 usec\nrounds: 1220"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1037541.9571370849,
            "unit": "iter/sec",
            "range": "stddev: 6.959238084396501e-8",
            "extra": "mean: 963.8164443578644 nsec\nrounds: 102260"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17047.88265617375,
            "unit": "iter/sec",
            "range": "stddev: 0.00000198769727339913",
            "extra": "mean: 58.658310839431905 usec\nrounds: 16938"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 185008.28501771047,
            "unit": "iter/sec",
            "range": "stddev: 4.181661983230259e-7",
            "extra": "mean: 5.405163341221567 usec\nrounds: 173612"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2045.0129850166672,
            "unit": "iter/sec",
            "range": "stddev: 0.000007508091446185991",
            "extra": "mean: 488.99445007281935 usec\nrounds: 2033"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 190056.04639039142,
            "unit": "iter/sec",
            "range": "stddev: 4.575982637047123e-7",
            "extra": "mean: 5.261605820979324 usec\nrounds: 135063"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2340.0494284122456,
            "unit": "iter/sec",
            "range": "stddev: 0.000005743752271700027",
            "extra": "mean: 427.3414005098659 usec\nrounds: 2352"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 193348.63957385387,
            "unit": "iter/sec",
            "range": "stddev: 3.9858246776845276e-7",
            "extra": "mean: 5.172004324437087 usec\nrounds: 179212"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2210.0781912320076,
            "unit": "iter/sec",
            "range": "stddev: 0.0000059991993291983265",
            "extra": "mean: 452.47267900623467 usec\nrounds: 2215"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 219433.74023436106,
            "unit": "iter/sec",
            "range": "stddev: 3.161239823333405e-7",
            "extra": "mean: 4.557184318746851 usec\nrounds: 110412"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2481.8645438785506,
            "unit": "iter/sec",
            "range": "stddev: 0.000005057628173241306",
            "extra": "mean: 402.922876055614 usec\nrounds: 2485"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10575.090764597679,
            "unit": "iter/sec",
            "range": "stddev: 0.000006666585053457132",
            "extra": "mean: 94.56183613550708 usec\nrounds: 10466"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 169.4858442229073,
            "unit": "iter/sec",
            "range": "stddev: 0.0001272138970110552",
            "extra": "mean: 5.90019776922964 msec\nrounds: 169"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "3645c23d83f768684f1de972d635f7bb935cf0b2",
          "message": "Add floating-point array benchmarks",
          "timestamp": "2024-03-14T23:32:28+01:00",
          "tree_id": "f945074951f1f6976717904c6ba06f1f80377632",
          "url": "https://github.com/apytypes/apytypes/commit/3645c23d83f768684f1de972d635f7bb935cf0b2"
        },
        "date": 1710455731550,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25206.779835364006,
            "unit": "iter/sec",
            "range": "stddev: 0.0000014623424121892164",
            "extra": "mean: 39.67186632054619 usec\nrounds: 24671"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 111600.81276801944,
            "unit": "iter/sec",
            "range": "stddev: 7.10038964695285e-7",
            "extra": "mean: 8.960508218507904 usec\nrounds: 107562"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1216.6611038713868,
            "unit": "iter/sec",
            "range": "stddev: 0.000026579469771188486",
            "extra": "mean: 821.9215661765003 usec\nrounds: 1224"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1020038.8050200236,
            "unit": "iter/sec",
            "range": "stddev: 5.8829573308483236e-8",
            "extra": "mean: 980.3548601079053 nsec\nrounds: 100111"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17038.044652275727,
            "unit": "iter/sec",
            "range": "stddev: 0.0000020116075292016743",
            "extra": "mean: 58.69218096376058 usec\nrounds: 17031"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 185231.21348148488,
            "unit": "iter/sec",
            "range": "stddev: 3.8354369105608237e-7",
            "extra": "mean: 5.398658148400872 usec\nrounds: 172414"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 1956.39923846647,
            "unit": "iter/sec",
            "range": "stddev: 0.00008861534662744222",
            "extra": "mean: 511.1431145229096 usec\nrounds: 2052"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 189868.52250601223,
            "unit": "iter/sec",
            "range": "stddev: 5.16150399824431e-7",
            "extra": "mean: 5.266802452567327 usec\nrounds: 136185"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2340.8726443162745,
            "unit": "iter/sec",
            "range": "stddev: 0.000005839742333054684",
            "extra": "mean: 427.19111713661016 usec\nrounds: 2305"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 195413.47477619306,
            "unit": "iter/sec",
            "range": "stddev: 4.275641716518049e-7",
            "extra": "mean: 5.117354374591104 usec\nrounds: 182482"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2205.2680630689047,
            "unit": "iter/sec",
            "range": "stddev: 0.0000257159346511986",
            "extra": "mean: 453.4596118933386 usec\nrounds: 2203"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 219005.5805263971,
            "unit": "iter/sec",
            "range": "stddev: 3.5913505497636e-7",
            "extra": "mean: 4.566093693121525 usec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2482.536610665282,
            "unit": "iter/sec",
            "range": "stddev: 0.000007182811767713702",
            "extra": "mean: 402.8137976712518 usec\nrounds: 2491"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10652.448984611558,
            "unit": "iter/sec",
            "range": "stddev: 0.000002866821776888217",
            "extra": "mean: 93.87512687876676 usec\nrounds: 10577"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 160.65043385496693,
            "unit": "iter/sec",
            "range": "stddev: 0.00032862908638000443",
            "extra": "mean: 6.2246952965143345 msec\nrounds: 172"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "02d2bd13ef7faaaef4c7acc1bc700225c53f5d5a",
          "message": "Add scalar benchmarks",
          "timestamp": "2024-03-14T23:49:40+01:00",
          "tree_id": "704141f81cde6793c35ac5ca1d86be8798eaaf06",
          "url": "https://github.com/apytypes/apytypes/commit/02d2bd13ef7faaaef4c7acc1bc700225c53f5d5a"
        },
        "date": 1710456766433,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25539.847800373736,
            "unit": "iter/sec",
            "range": "stddev: 0.0000020694617808912105",
            "extra": "mean: 39.15450114723732 usec\nrounds: 24404"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 115676.09675072042,
            "unit": "iter/sec",
            "range": "stddev: 6.895645137081976e-7",
            "extra": "mean: 8.644828344744198 usec\nrounds: 119105"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1251.1800025298437,
            "unit": "iter/sec",
            "range": "stddev: 0.00003753993990989272",
            "extra": "mean: 799.2455106204014 usec\nrounds: 1224"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1048828.4185754089,
            "unit": "iter/sec",
            "range": "stddev: 8.613468803699901e-8",
            "extra": "mean: 953.4447983000865 nsec\nrounds: 100919"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17714.21724370458,
            "unit": "iter/sec",
            "range": "stddev: 0.0000033898844241935248",
            "extra": "mean: 56.45183110506268 usec\nrounds: 16981"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 191580.14796516308,
            "unit": "iter/sec",
            "range": "stddev: 5.321326698227152e-7",
            "extra": "mean: 5.219747508399669 usec\nrounds: 173281"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2103.8923953511085,
            "unit": "iter/sec",
            "range": "stddev: 0.00002549561645633586",
            "extra": "mean: 475.3094798049854 usec\nrounds: 2055"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 194567.81671769402,
            "unit": "iter/sec",
            "range": "stddev: 4.461424720305177e-7",
            "extra": "mean: 5.1395961411796005 usec\nrounds: 131338"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2414.3946653577473,
            "unit": "iter/sec",
            "range": "stddev: 0.00001641162413795717",
            "extra": "mean: 414.18249234402913 usec\nrounds: 2547"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 199698.29601530943,
            "unit": "iter/sec",
            "range": "stddev: 4.985991171360193e-7",
            "extra": "mean: 5.007553994969177 usec\nrounds: 181489"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2292.84817450372,
            "unit": "iter/sec",
            "range": "stddev: 0.00001750113158153916",
            "extra": "mean: 436.1387775779994 usec\nrounds: 2221"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 226617.7921299821,
            "unit": "iter/sec",
            "range": "stddev: 3.373452610700121e-7",
            "extra": "mean: 4.412716188790799 usec\nrounds: 109446"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2544.162850362806,
            "unit": "iter/sec",
            "range": "stddev: 0.000019941231324790687",
            "extra": "mean: 393.0566000747149 usec\nrounds: 2678"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10741.475489350887,
            "unit": "iter/sec",
            "range": "stddev: 0.00000576158839476251",
            "extra": "mean: 93.09707972535071 usec\nrounds: 10486"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 172.90861393895992,
            "unit": "iter/sec",
            "range": "stddev: 0.00012679085323185916",
            "extra": "mean: 5.783401863096417 msec\nrounds: 168"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "8cb26a418e61f82a8962ffb3999e960269b37c07",
          "message": "Fix stupid error",
          "timestamp": "2024-03-14T23:54:22+01:00",
          "tree_id": "76800c16ba9dd5c629c6f3da76296ba2b5917c02",
          "url": "https://github.com/apytypes/apytypes/commit/8cb26a418e61f82a8962ffb3999e960269b37c07"
        },
        "date": 1710457046638,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24135.471817563357,
            "unit": "iter/sec",
            "range": "stddev: 0.000010772972997462777",
            "extra": "mean: 41.43279267788339 usec\nrounds: 24093"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 113166.14962679948,
            "unit": "iter/sec",
            "range": "stddev: 6.154420511941058e-7",
            "extra": "mean: 8.836564673250884 usec\nrounds: 109566"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1219.4217055536935,
            "unit": "iter/sec",
            "range": "stddev: 0.000007514669559945629",
            "extra": "mean: 820.0608497008323 usec\nrounds: 1171"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1034402.961037324,
            "unit": "iter/sec",
            "range": "stddev: 6.309012559533186e-8",
            "extra": "mean: 966.7412388274272 nsec\nrounds: 101225"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 16942.830295530228,
            "unit": "iter/sec",
            "range": "stddev: 0.000002244484284899239",
            "extra": "mean: 59.02201595348653 usec\nrounds: 16987"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 183472.29539546176,
            "unit": "iter/sec",
            "range": "stddev: 4.6230143516857975e-7",
            "extra": "mean: 5.450414177489684 usec\nrounds: 172385"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2027.1466329238417,
            "unit": "iter/sec",
            "range": "stddev: 0.00004601427930823374",
            "extra": "mean: 493.304225633474 usec\nrounds: 2052"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 190257.87770603332,
            "unit": "iter/sec",
            "range": "stddev: 3.673105002010605e-7",
            "extra": "mean: 5.2560241502593446 usec\nrounds: 134699"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2339.0006728506205,
            "unit": "iter/sec",
            "range": "stddev: 0.000016543470280440584",
            "extra": "mean: 427.5330108311878 usec\nrounds: 2308"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 195702.33802061656,
            "unit": "iter/sec",
            "range": "stddev: 4.11200709139123e-7",
            "extra": "mean: 5.109800987122869 usec\nrounds: 179501"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2209.7915225040883,
            "unit": "iter/sec",
            "range": "stddev: 0.000016741059817362023",
            "extra": "mean: 452.5313767458124 usec\nrounds: 2219"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 222689.63295439543,
            "unit": "iter/sec",
            "range": "stddev: 3.071090506222853e-7",
            "extra": "mean: 4.490554799220446 usec\nrounds: 110914"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2485.066423536323,
            "unit": "iter/sec",
            "range": "stddev: 0.000005221340094856702",
            "extra": "mean: 402.4037307529875 usec\nrounds: 2481"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10719.913683527348,
            "unit": "iter/sec",
            "range": "stddev: 0.000003939151863122756",
            "extra": "mean: 93.28433320658546 usec\nrounds: 10546"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 173.83183102929158,
            "unit": "iter/sec",
            "range": "stddev: 0.00004976082134221821",
            "extra": "mean: 5.75268634103897 msec\nrounds: 173"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "d338d962f5c8961c92014489e294def34e6bb6b3",
          "message": "Remove failing benchmarks",
          "timestamp": "2024-03-15T00:01:02+01:00",
          "tree_id": "2daf9ddefd16aa714a5dd19fbc21a74046065b2e",
          "url": "https://github.com/apytypes/apytypes/commit/d338d962f5c8961c92014489e294def34e6bb6b3"
        },
        "date": 1710457445060,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25177.984821176004,
            "unit": "iter/sec",
            "range": "stddev: 0.0000013792444121364905",
            "extra": "mean: 39.71723738426228 usec\nrounds: 24315"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 111872.57480808267,
            "unit": "iter/sec",
            "range": "stddev: 5.372634130172195e-7",
            "extra": "mean: 8.938741257323336 usec\nrounds: 108378"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1218.467271036996,
            "unit": "iter/sec",
            "range": "stddev: 0.000009140662941954434",
            "extra": "mean: 820.7032094911619 usec\nrounds: 1222"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 866780.5663776062,
            "unit": "iter/sec",
            "range": "stddev: 6.999554360495096e-7",
            "extra": "mean: 1.153694532145719 usec\nrounds: 101740"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 12081.037929116643,
            "unit": "iter/sec",
            "range": "stddev: 0.000025731402683304292",
            "extra": "mean: 82.77434487560784 usec\nrounds: 16548"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 184717.53956662095,
            "unit": "iter/sec",
            "range": "stddev: 4.6665982553525465e-7",
            "extra": "mean: 5.413671069602658 usec\nrounds: 177305"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2045.3824575478425,
            "unit": "iter/sec",
            "range": "stddev: 0.000004806679300209928",
            "extra": "mean: 488.9061193958194 usec\nrounds: 2052"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 190417.95676200828,
            "unit": "iter/sec",
            "range": "stddev: 4.45241809503928e-7",
            "extra": "mean: 5.251605557609458 usec\nrounds: 133619"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2331.195637354057,
            "unit": "iter/sec",
            "range": "stddev: 0.000029312027876147293",
            "extra": "mean: 428.96442665576336 usec\nrounds: 2311"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 195896.28273274648,
            "unit": "iter/sec",
            "range": "stddev: 3.832851302375324e-7",
            "extra": "mean: 5.104742091325236 usec\nrounds: 178540"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2212.9278674318502,
            "unit": "iter/sec",
            "range": "stddev: 0.00000537075802130501",
            "extra": "mean: 451.8900117429138 usec\nrounds: 2214"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 218215.12387739192,
            "unit": "iter/sec",
            "range": "stddev: 3.86037219806466e-7",
            "extra": "mean: 4.582633789222913 usec\nrounds: 110902"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2480.79695936655,
            "unit": "iter/sec",
            "range": "stddev: 0.000005246674953120088",
            "extra": "mean: 403.0962696178657 usec\nrounds: 2485"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10673.660807337501,
            "unit": "iter/sec",
            "range": "stddev: 0.000002646134545428889",
            "extra": "mean: 93.68856834129112 usec\nrounds: 10550"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 162.97870423877413,
            "unit": "iter/sec",
            "range": "stddev: 0.00034377894666850524",
            "extra": "mean: 6.135770956522864 msec\nrounds: 161"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "70df93b98047c99b62eb6d4a322f9a3adc5001be",
          "message": "Add more conversion benchmarks",
          "timestamp": "2024-03-15T00:34:48+01:00",
          "tree_id": "3b5bc3581009c06b1f47c5d6037abbdc4bcfe5e2",
          "url": "https://github.com/apytypes/apytypes/commit/70df93b98047c99b62eb6d4a322f9a3adc5001be"
        },
        "date": 1710459473391,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25135.41131449691,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018402440647002817",
            "extra": "mean: 39.78450909308365 usec\nrounds: 24579"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 112760.88652864534,
            "unit": "iter/sec",
            "range": "stddev: 8.369199048293258e-7",
            "extra": "mean: 8.868323323672733 usec\nrounds: 109927"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1220.0267524894698,
            "unit": "iter/sec",
            "range": "stddev: 0.000014731852362667125",
            "extra": "mean: 819.6541575498208 usec\nrounds: 1225"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1035611.4928605414,
            "unit": "iter/sec",
            "range": "stddev: 1.3969980207075815e-7",
            "extra": "mean: 965.6130768091374 nsec\nrounds: 103542"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17053.326613042023,
            "unit": "iter/sec",
            "range": "stddev: 0.000002417790882919777",
            "extra": "mean: 58.63958526632694 usec\nrounds: 16683"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 185260.09585916682,
            "unit": "iter/sec",
            "range": "stddev: 4.296835562579058e-7",
            "extra": "mean: 5.397816488015809 usec\nrounds: 174490"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2042.3711481418468,
            "unit": "iter/sec",
            "range": "stddev: 0.000019696664759244056",
            "extra": "mean: 489.62697152758057 usec\nrounds: 2037"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 189643.35671230248,
            "unit": "iter/sec",
            "range": "stddev: 5.164579936564847e-7",
            "extra": "mean: 5.273055789225695 usec\nrounds: 130993"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2341.4516422478746,
            "unit": "iter/sec",
            "range": "stddev: 0.000007325285816536643",
            "extra": "mean: 427.0854806294293 usec\nrounds: 2349"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 195005.6283731044,
            "unit": "iter/sec",
            "range": "stddev: 4.875222392333083e-7",
            "extra": "mean: 5.128057114775678 usec\nrounds: 180181"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2204.5657349029275,
            "unit": "iter/sec",
            "range": "stddev: 0.000028850985170418832",
            "extra": "mean: 453.60407456574774 usec\nrounds: 2186"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 222374.64663935496,
            "unit": "iter/sec",
            "range": "stddev: 3.352898121958151e-7",
            "extra": "mean: 4.4969155212275185 usec\nrounds: 109927"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2483.687117514375,
            "unit": "iter/sec",
            "range": "stddev: 0.000005721078149579734",
            "extra": "mean: 402.62720410644164 usec\nrounds: 2484"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10610.442906067121,
            "unit": "iter/sec",
            "range": "stddev: 0.000002321461480974937",
            "extra": "mean: 94.24677262324208 usec\nrounds: 10454"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 170.01777450813918,
            "unit": "iter/sec",
            "range": "stddev: 0.00011981134739472483",
            "extra": "mean: 5.881737970591583 msec\nrounds: 170"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 55797.655886207125,
            "unit": "iter/sec",
            "range": "stddev: 9.453462263164993e-7",
            "extra": "mean: 17.92189983821873 usec\nrounds: 50718"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 614.0543524113751,
            "unit": "iter/sec",
            "range": "stddev: 0.000030927563851208105",
            "extra": "mean: 1.6285203354931475 msec\nrounds: 617"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "82bfe149914ec2681af1b24d7ae7885c16e1f954",
          "message": "APyFixed: specialize arithmetic when result fits single limb",
          "timestamp": "2024-03-15T09:25:20+01:00",
          "tree_id": "d6d91ca93a58a9965802be1f5c31bdf7e249a5ae",
          "url": "https://github.com/apytypes/apytypes/commit/82bfe149914ec2681af1b24d7ae7885c16e1f954"
        },
        "date": 1710492067457,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25357.18231706514,
            "unit": "iter/sec",
            "range": "stddev: 0.00000188101184559821",
            "extra": "mean: 39.4365583484806 usec\nrounds: 24585"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 111481.39428569945,
            "unit": "iter/sec",
            "range": "stddev: 6.388630387000366e-7",
            "extra": "mean: 8.970106683786582 usec\nrounds: 106633"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1196.4198107214604,
            "unit": "iter/sec",
            "range": "stddev: 0.000026529331221832462",
            "extra": "mean: 835.827015767136 usec\nrounds: 1205"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1031477.694196251,
            "unit": "iter/sec",
            "range": "stddev: 1.0480993952243125e-7",
            "extra": "mean: 969.4829133258019 nsec\nrounds: 103531"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17326.79802556941,
            "unit": "iter/sec",
            "range": "stddev: 0.0000024999604582676384",
            "extra": "mean: 57.71406803058969 usec\nrounds: 16669"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 180168.01976033565,
            "unit": "iter/sec",
            "range": "stddev: 7.160764222457564e-7",
            "extra": "mean: 5.55037459661391 usec\nrounds: 168891"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 1992.4987069198519,
            "unit": "iter/sec",
            "range": "stddev: 0.0000070149580427846395",
            "extra": "mean: 501.8823834249167 usec\nrounds: 2003"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 188943.56915547923,
            "unit": "iter/sec",
            "range": "stddev: 4.2912691131450534e-7",
            "extra": "mean: 5.292585529476861 usec\nrounds: 129283"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2275.4448224394196,
            "unit": "iter/sec",
            "range": "stddev: 0.0000061530788863103184",
            "extra": "mean: 439.474510714761 usec\nrounds: 2240"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 195164.87968539126,
            "unit": "iter/sec",
            "range": "stddev: 4.514478504343984e-7",
            "extra": "mean: 5.123872705027744 usec\nrounds: 180832"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2208.48086057324,
            "unit": "iter/sec",
            "range": "stddev: 0.000006345414566342235",
            "extra": "mean: 452.7999394753355 usec\nrounds: 2214"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 220691.11747624038,
            "unit": "iter/sec",
            "range": "stddev: 3.457888428275122e-7",
            "extra": "mean: 4.531219975845471 usec\nrounds: 110669"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2471.9517347879073,
            "unit": "iter/sec",
            "range": "stddev: 0.000014844111992252168",
            "extra": "mean: 404.53864285736125 usec\nrounds: 2478"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10622.785093163102,
            "unit": "iter/sec",
            "range": "stddev: 0.0000037319795750210866",
            "extra": "mean: 94.13727108567855 usec\nrounds: 10469"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 172.2497937225468,
            "unit": "iter/sec",
            "range": "stddev: 0.00013785486234271703",
            "extra": "mean: 5.805522191862597 msec\nrounds: 172"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 62186.98348813706,
            "unit": "iter/sec",
            "range": "stddev: 0.000001077053261449218",
            "extra": "mean: 16.080535570450404 usec\nrounds: 56648"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 693.0597770631056,
            "unit": "iter/sec",
            "range": "stddev: 0.00001091463854050186",
            "extra": "mean: 1.4428769827583667 msec\nrounds: 696"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "f329bbd75bc608b8770b1d73155b9bbae3635fa0",
          "message": "APyFixedArray: return APyFixed when accessing vector APyFixedArrays",
          "timestamp": "2024-03-15T10:03:41+01:00",
          "tree_id": "7ed451bf7ff99dc7e810a030aa9439781ce81db0",
          "url": "https://github.com/apytypes/apytypes/commit/f329bbd75bc608b8770b1d73155b9bbae3635fa0"
        },
        "date": 1710495255455,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24839.35459044055,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016983814074986397",
            "extra": "mean: 40.25869498174687 usec\nrounds: 24251"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 111348.45156129474,
            "unit": "iter/sec",
            "range": "stddev: 5.696683106001646e-7",
            "extra": "mean: 8.980816400931477 usec\nrounds: 106079"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1172.354405580644,
            "unit": "iter/sec",
            "range": "stddev: 0.000021131622942963423",
            "extra": "mean: 852.9843835957778 usec\nrounds: 1207"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1013769.7349193633,
            "unit": "iter/sec",
            "range": "stddev: 5.5015321795592404e-8",
            "extra": "mean: 986.4172953235418 nsec\nrounds: 100929"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17427.776492456735,
            "unit": "iter/sec",
            "range": "stddev: 0.0000026500621232153265",
            "extra": "mean: 57.37966632936967 usec\nrounds: 16801"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 184238.0586972012,
            "unit": "iter/sec",
            "range": "stddev: 4.954959906416332e-7",
            "extra": "mean: 5.427760187397107 usec\nrounds: 174186"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2025.8513422594926,
            "unit": "iter/sec",
            "range": "stddev: 0.000012234256598390724",
            "extra": "mean: 493.61963493563655 usec\nrounds: 2038"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 193389.45254957498,
            "unit": "iter/sec",
            "range": "stddev: 3.5185107490817353e-7",
            "extra": "mean: 5.170912822888581 usec\nrounds: 133441"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2132.4993133163007,
            "unit": "iter/sec",
            "range": "stddev: 0.000008677457255221414",
            "extra": "mean: 468.93332802291786 usec\nrounds: 2134"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 193085.36507329418,
            "unit": "iter/sec",
            "range": "stddev: 6.703764729576765e-7",
            "extra": "mean: 5.179056422118815 usec\nrounds: 176367"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2207.649674209077,
            "unit": "iter/sec",
            "range": "stddev: 0.00001657941536311513",
            "extra": "mean: 452.9704199368791 usec\nrounds: 2217"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 220591.08605094225,
            "unit": "iter/sec",
            "range": "stddev: 3.3355962755327234e-7",
            "extra": "mean: 4.533274747870205 usec\nrounds: 110767"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2479.7458552593025,
            "unit": "iter/sec",
            "range": "stddev: 0.0000058735046904030145",
            "extra": "mean: 403.2671323470896 usec\nrounds: 2433"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10636.301722418708,
            "unit": "iter/sec",
            "range": "stddev: 0.000004190908553480521",
            "extra": "mean: 94.01764129088647 usec\nrounds: 10443"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 171.94131952259664,
            "unit": "iter/sec",
            "range": "stddev: 0.00005976788997460853",
            "extra": "mean: 5.81593768604631 msec\nrounds: 172"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 61324.25983377436,
            "unit": "iter/sec",
            "range": "stddev: 0.000001140752919750671",
            "extra": "mean: 16.30676020730787 usec\nrounds: 55206"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 687.8227191945977,
            "unit": "iter/sec",
            "range": "stddev: 0.000013237728055786749",
            "extra": "mean: 1.4538629970974855 msec\nrounds: 689"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "760013dc6684d9980a24c347ceebbd0c16fce71d",
          "message": "APyFloatArray: return APyFloat when accessing vector APyFloatArrays",
          "timestamp": "2024-03-15T10:29:16+01:00",
          "tree_id": "82e23fc0decab95cce932582050670ccb1005077",
          "url": "https://github.com/apytypes/apytypes/commit/760013dc6684d9980a24c347ceebbd0c16fce71d"
        },
        "date": 1710495991315,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24706.798116790567,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016237349826675643",
            "extra": "mean: 40.47469021574297 usec\nrounds: 24304"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 107859.3070400558,
            "unit": "iter/sec",
            "range": "stddev: 6.954444653428281e-7",
            "extra": "mean: 9.271337146905914 usec\nrounds: 104189"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1184.2522196691486,
            "unit": "iter/sec",
            "range": "stddev: 0.000009221159346308684",
            "extra": "mean: 844.4147145270927 usec\nrounds: 1184"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1004462.4450489315,
            "unit": "iter/sec",
            "range": "stddev: 6.928552202901764e-8",
            "extra": "mean: 995.5573798991536 nsec\nrounds: 100523"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17233.27684986796,
            "unit": "iter/sec",
            "range": "stddev: 0.000005608439642805994",
            "extra": "mean: 58.027269492143155 usec\nrounds: 17648"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 179228.2240820881,
            "unit": "iter/sec",
            "range": "stddev: 5.160082425058798e-7",
            "extra": "mean: 5.579478372457627 usec\nrounds: 166362"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 1992.5499196756516,
            "unit": "iter/sec",
            "range": "stddev: 0.000018416817877123667",
            "extra": "mean: 501.8694839840102 usec\nrounds: 1998"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 184808.24148453376,
            "unit": "iter/sec",
            "range": "stddev: 4.441223469921828e-7",
            "extra": "mean: 5.411014097462142 usec\nrounds: 127470"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2273.190499475126,
            "unit": "iter/sec",
            "range": "stddev: 0.000016268428095052308",
            "extra": "mean: 439.91033757659 usec\nrounds: 2278"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 190406.60699947175,
            "unit": "iter/sec",
            "range": "stddev: 0.0000014472490578621333",
            "extra": "mean: 5.251918595465409 usec\nrounds: 175408"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2208.2124142835855,
            "unit": "iter/sec",
            "range": "stddev: 0.000005366205114641383",
            "extra": "mean: 452.8549851144786 usec\nrounds: 2217"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 220885.0971479486,
            "unit": "iter/sec",
            "range": "stddev: 3.2323027213035323e-7",
            "extra": "mean: 4.527240691707695 usec\nrounds: 108720"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2476.0724267735604,
            "unit": "iter/sec",
            "range": "stddev: 0.000019213473005592553",
            "extra": "mean: 403.8654076460305 usec\nrounds: 2485"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10578.330652486597,
            "unit": "iter/sec",
            "range": "stddev: 0.000003864904163785861",
            "extra": "mean: 94.53287412271753 usec\nrounds: 10685"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 168.79890850990282,
            "unit": "iter/sec",
            "range": "stddev: 0.0001156024705438577",
            "extra": "mean: 5.924208923076856 msec\nrounds: 169"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 61926.94607370323,
            "unit": "iter/sec",
            "range": "stddev: 9.639425541800313e-7",
            "extra": "mean: 16.14805934091818 usec\nrounds: 54903"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 692.2099188042438,
            "unit": "iter/sec",
            "range": "stddev: 0.0000369233857303933",
            "extra": "mean: 1.444648469827545 msec\nrounds: 696"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "e7e2ccbf4ad2099fb3453ebb475036a26a8d0dc4",
          "message": "APyFixed::APyFixed(int, int, _IT, _IT): support short vector initialization",
          "timestamp": "2024-03-15T14:45:15+01:00",
          "tree_id": "70370597be0b422ef6e0f71486598e54c8c677b4",
          "url": "https://github.com/apytypes/apytypes/commit/e7e2ccbf4ad2099fb3453ebb475036a26a8d0dc4"
        },
        "date": 1710510802317,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25139.51097039219,
            "unit": "iter/sec",
            "range": "stddev: 0.000001948089297281768",
            "extra": "mean: 39.77802118655928 usec\nrounds: 24591"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 113050.54367817221,
            "unit": "iter/sec",
            "range": "stddev: 6.227418717831574e-7",
            "extra": "mean: 8.845600980449596 usec\nrounds: 107090"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1219.819487627874,
            "unit": "iter/sec",
            "range": "stddev: 0.000007156587707316811",
            "extra": "mean: 819.7934285708563 usec\nrounds: 1225"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1012182.9560237664,
            "unit": "iter/sec",
            "range": "stddev: 7.340038690590229e-8",
            "extra": "mean: 987.9636819102506 nsec\nrounds: 102376"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 16883.98259023167,
            "unit": "iter/sec",
            "range": "stddev: 0.000002075913276185908",
            "extra": "mean: 59.227732240055495 usec\nrounds: 16526"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 181260.16754520082,
            "unit": "iter/sec",
            "range": "stddev: 4.457658893104997e-7",
            "extra": "mean: 5.516931897078988 usec\nrounds: 173311"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2038.9212725629925,
            "unit": "iter/sec",
            "range": "stddev: 0.000005723698832096154",
            "extra": "mean: 490.45542535488204 usec\nrounds: 2043"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 191761.35446096858,
            "unit": "iter/sec",
            "range": "stddev: 4.785571260108e-7",
            "extra": "mean: 5.214815064333213 usec\nrounds: 133798"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2338.3239099230304,
            "unit": "iter/sec",
            "range": "stddev: 0.000006478966385682032",
            "extra": "mean: 427.65674838988264 usec\nrounds: 2329"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 195249.803600137,
            "unit": "iter/sec",
            "range": "stddev: 4.310145615654434e-7",
            "extra": "mean: 5.121644076262202 usec\nrounds: 178572"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2209.3107952795312,
            "unit": "iter/sec",
            "range": "stddev: 0.000012562093533685762",
            "extra": "mean: 452.62984372168233 usec\nrounds: 2214"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 221476.97834508726,
            "unit": "iter/sec",
            "range": "stddev: 3.3681591892339237e-7",
            "extra": "mean: 4.5151419685791545 usec\nrounds: 109806"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2480.7522378954322,
            "unit": "iter/sec",
            "range": "stddev: 0.000006241624348317554",
            "extra": "mean: 403.1035363887684 usec\nrounds: 2487"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10608.83021000007,
            "unit": "iter/sec",
            "range": "stddev: 0.000004485296898726515",
            "extra": "mean: 94.2610994996774 usec\nrounds: 10593"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 166.66204493630894,
            "unit": "iter/sec",
            "range": "stddev: 0.00007205013996782157",
            "extra": "mean: 6.000166386906851 msec\nrounds: 168"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 61007.59878220812,
            "unit": "iter/sec",
            "range": "stddev: 0.000001108576258176594",
            "extra": "mean: 16.391400742879817 usec\nrounds: 54933"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 681.3441362278546,
            "unit": "iter/sec",
            "range": "stddev: 0.00001122851146934745",
            "extra": "mean: 1.467687100877288 msec\nrounds: 684"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "1a2af23f9e038c24d1f056f8a80e56302548c759",
          "message": "src: python_sequence_extract_shape, filter strings",
          "timestamp": "2024-03-15T15:21:37+01:00",
          "tree_id": "e1147f625d2fed813b4f8f2268c7fe02924d5ccf",
          "url": "https://github.com/apytypes/apytypes/commit/1a2af23f9e038c24d1f056f8a80e56302548c759"
        },
        "date": 1710514081896,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24946.82604666982,
            "unit": "iter/sec",
            "range": "stddev: 0.0000037047373563677985",
            "extra": "mean: 40.085259669074865 usec\nrounds: 24381"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 114355.58004881936,
            "unit": "iter/sec",
            "range": "stddev: 6.545773997474552e-7",
            "extra": "mean: 8.744654170553737 usec\nrounds: 109927"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1235.5968289714306,
            "unit": "iter/sec",
            "range": "stddev: 0.000010677042620484412",
            "extra": "mean: 809.3254826758074 usec\nrounds: 1241"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1034233.5668189105,
            "unit": "iter/sec",
            "range": "stddev: 6.916530476120216e-8",
            "extra": "mean: 966.8995786665276 nsec\nrounds: 103008"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17288.67781712107,
            "unit": "iter/sec",
            "range": "stddev: 0.0000024716991914195144",
            "extra": "mean: 57.84132312360489 usec\nrounds: 17402"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 188375.68198992117,
            "unit": "iter/sec",
            "range": "stddev: 6.141688090917137e-7",
            "extra": "mean: 5.308540834126901 usec\nrounds: 176960"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2084.5545273885355,
            "unit": "iter/sec",
            "range": "stddev: 0.00001744469478001872",
            "extra": "mean: 479.7188017205617 usec\nrounds: 2093"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 194414.92892512702,
            "unit": "iter/sec",
            "range": "stddev: 4.6708277043764055e-7",
            "extra": "mean: 5.1436379167420805 usec\nrounds: 137855"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2338.148256166127,
            "unit": "iter/sec",
            "range": "stddev: 0.000016290663685778245",
            "extra": "mean: 427.6888761706261 usec\nrounds: 2350"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 195070.7079000955,
            "unit": "iter/sec",
            "range": "stddev: 4.1902899024878846e-7",
            "extra": "mean: 5.1263462913773035 usec\nrounds: 182150"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2268.966061150782,
            "unit": "iter/sec",
            "range": "stddev: 0.000015583759598816155",
            "extra": "mean: 440.7293776323902 usec\nrounds: 2280"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 217799.4375172109,
            "unit": "iter/sec",
            "range": "stddev: 4.188054881285022e-7",
            "extra": "mean: 4.591380085272159 usec\nrounds: 196890"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2476.935152884676,
            "unit": "iter/sec",
            "range": "stddev: 0.00001429670069708891",
            "extra": "mean: 403.7247397596925 usec\nrounds: 2490"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10680.66688066921,
            "unit": "iter/sec",
            "range": "stddev: 0.000003610212265448103",
            "extra": "mean: 93.62711253637973 usec\nrounds: 10370"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 167.83078327009085,
            "unit": "iter/sec",
            "range": "stddev: 0.00010379289222628848",
            "extra": "mean: 5.958382488096331 msec\nrounds: 168"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 62641.35737893698,
            "unit": "iter/sec",
            "range": "stddev: 0.0000011972080406699744",
            "extra": "mean: 15.963894172195062 usec\nrounds: 56488"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 697.8378267440316,
            "unit": "iter/sec",
            "range": "stddev: 0.000011528157548916452",
            "extra": "mean: 1.4329976990009199 msec\nrounds: 701"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "ba7fb25e8c9c94902e2b8bc6cdfe5fb2ba19546f",
          "message": "Add relative benchmark plot",
          "timestamp": "2024-03-15T17:43:34+01:00",
          "tree_id": "c2b7135118f18b3b808d84ad4c1dc5e488d8ee3f",
          "url": "https://github.com/apytypes/apytypes/commit/ba7fb25e8c9c94902e2b8bc6cdfe5fb2ba19546f"
        },
        "date": 1710521196082,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25107.035308863848,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015497202994727587",
            "extra": "mean: 39.82947359965506 usec\nrounds: 24375"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 114713.4629805092,
            "unit": "iter/sec",
            "range": "stddev: 6.286042592957639e-7",
            "extra": "mean: 8.717372608391296 usec\nrounds: 110902"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1206.266288340054,
            "unit": "iter/sec",
            "range": "stddev: 0.00001934294977443079",
            "extra": "mean: 829.004349757716 usec\nrounds: 1238"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1033400.1229410578,
            "unit": "iter/sec",
            "range": "stddev: 6.46231031439309e-8",
            "extra": "mean: 967.679389425618 nsec\nrounds: 102166"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17331.633311232934,
            "unit": "iter/sec",
            "range": "stddev: 0.0000025840039054691117",
            "extra": "mean: 57.69796660490634 usec\nrounds: 17218"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 188377.07052462487,
            "unit": "iter/sec",
            "range": "stddev: 5.133039363671598e-7",
            "extra": "mean: 5.30850170466622 usec\nrounds: 175101"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2059.8983744973452,
            "unit": "iter/sec",
            "range": "stddev: 0.00001639465023475658",
            "extra": "mean: 485.46084233112674 usec\nrounds: 2093"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 194900.06334813882,
            "unit": "iter/sec",
            "range": "stddev: 4.6166713066467067e-7",
            "extra": "mean: 5.130834658651481 usec\nrounds: 132556"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2342.2960524305017,
            "unit": "iter/sec",
            "range": "stddev: 0.000005818593610432635",
            "extra": "mean: 426.93151404253206 usec\nrounds: 2350"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 198857.93646969358,
            "unit": "iter/sec",
            "range": "stddev: 4.419926611502871e-7",
            "extra": "mean: 5.028715563245334 usec\nrounds: 181819"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2271.077183767988,
            "unit": "iter/sec",
            "range": "stddev: 0.000006635391704686054",
            "extra": "mean: 440.319689329484 usec\nrounds: 2221"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 222155.1304027306,
            "unit": "iter/sec",
            "range": "stddev: 3.2776941950526626e-7",
            "extra": "mean: 4.5013590196506605 usec\nrounds: 110792"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2482.1645320640496,
            "unit": "iter/sec",
            "range": "stddev: 0.000005669763031265388",
            "extra": "mean: 402.8741798064642 usec\nrounds: 2486"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10641.16690220875,
            "unit": "iter/sec",
            "range": "stddev: 0.000004229988903943968",
            "extra": "mean: 93.97465608705315 usec\nrounds: 8633"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 165.39132267799206,
            "unit": "iter/sec",
            "range": "stddev: 0.00012133877801520216",
            "extra": "mean: 6.046266417174411 msec\nrounds: 163"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 63116.73094816766,
            "unit": "iter/sec",
            "range": "stddev: 9.200261845461251e-7",
            "extra": "mean: 15.843659596711591 usec\nrounds: 56389"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 696.8986780238242,
            "unit": "iter/sec",
            "range": "stddev: 0.000043303130708165396",
            "extra": "mean: 1.434928823276967 msec\nrounds: 696"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "fd93e3629cd4d50702706a40146f8d7ca4afc002",
          "message": "Special case short negations",
          "timestamp": "2024-03-15T18:35:57+01:00",
          "tree_id": "9f5f101b34999448b5e13a4ac81d084c5952c5ef",
          "url": "https://github.com/apytypes/apytypes/commit/fd93e3629cd4d50702706a40146f8d7ca4afc002"
        },
        "date": 1710524327961,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25943.219938950217,
            "unit": "iter/sec",
            "range": "stddev: 0.0000019807889512779196",
            "extra": "mean: 38.545716466699496 usec\nrounds: 26660"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 116579.30017661458,
            "unit": "iter/sec",
            "range": "stddev: 5.751253943445454e-7",
            "extra": "mean: 8.577852144291708 usec\nrounds: 110060"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1279.2916972394178,
            "unit": "iter/sec",
            "range": "stddev: 0.00002978516728062616",
            "extra": "mean: 781.6825530548655 usec\nrounds: 1244"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1065033.1551774787,
            "unit": "iter/sec",
            "range": "stddev: 7.791984010309233e-8",
            "extra": "mean: 938.9379054902208 nsec\nrounds: 103853"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17324.127899290404,
            "unit": "iter/sec",
            "range": "stddev: 0.00000277496940265667",
            "extra": "mean: 57.72296336146076 usec\nrounds: 17004"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 192168.84091421834,
            "unit": "iter/sec",
            "range": "stddev: 5.457717782447828e-7",
            "extra": "mean: 5.20375725451967 usec\nrounds: 171204"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2074.8221790808966,
            "unit": "iter/sec",
            "range": "stddev: 0.000028872593119953736",
            "extra": "mean: 481.9690140593057 usec\nrounds: 2205"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 199283.86727393823,
            "unit": "iter/sec",
            "range": "stddev: 4.32622743388475e-7",
            "extra": "mean: 5.017967654277738 usec\nrounds: 135999"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2403.1685441190743,
            "unit": "iter/sec",
            "range": "stddev: 0.000016155976725866277",
            "extra": "mean: 416.11729749340924 usec\nrounds: 2353"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 200131.86555070375,
            "unit": "iter/sec",
            "range": "stddev: 4.327183969753737e-7",
            "extra": "mean: 4.996705533365692 usec\nrounds: 178859"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2290.737907373549,
            "unit": "iter/sec",
            "range": "stddev: 0.000018126383857400366",
            "extra": "mean: 436.5405561156284 usec\nrounds: 2379"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 222809.05450305156,
            "unit": "iter/sec",
            "range": "stddev: 3.111332801098066e-7",
            "extra": "mean: 4.488147944572441 usec\nrounds: 110048"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2575.545068498948,
            "unit": "iter/sec",
            "range": "stddev: 0.000026087139092155307",
            "extra": "mean: 388.2673272663055 usec\nrounds: 2692"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10994.498860370499,
            "unit": "iter/sec",
            "range": "stddev: 0.0000042274674357166664",
            "extra": "mean: 90.9545776210396 usec\nrounds: 9920"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 170.39982484831688,
            "unit": "iter/sec",
            "range": "stddev: 0.00020945381315529677",
            "extra": "mean: 5.868550633136859 msec\nrounds: 169"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 59558.39334801813,
            "unit": "iter/sec",
            "range": "stddev: 0.00000108389449560388",
            "extra": "mean: 16.79024472934806 usec\nrounds: 52176"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 647.2439828432119,
            "unit": "iter/sec",
            "range": "stddev: 0.00007510409487006312",
            "extra": "mean: 1.5450124319537157 msec\nrounds: 676"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "dadfb6c0a1245b2d954e945b0bf6cca7715fd61d",
          "message": "Special case short abs",
          "timestamp": "2024-03-15T18:55:40+01:00",
          "tree_id": "ee091c534560f949b05acda375c11f87d6d15196",
          "url": "https://github.com/apytypes/apytypes/commit/dadfb6c0a1245b2d954e945b0bf6cca7715fd61d"
        },
        "date": 1710525516681,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 23579.61368461513,
            "unit": "iter/sec",
            "range": "stddev: 0.0000013461346100821182",
            "extra": "mean: 42.409515837507755 usec\nrounds: 23015"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 104166.90744179724,
            "unit": "iter/sec",
            "range": "stddev: 6.05157027319184e-7",
            "extra": "mean: 9.599977810215258 usec\nrounds: 99325"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1113.7558626009927,
            "unit": "iter/sec",
            "range": "stddev: 0.00002772108677416991",
            "extra": "mean: 897.8628383285591 usec\nrounds: 1101"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1024585.4618896748,
            "unit": "iter/sec",
            "range": "stddev: 7.014673144914697e-8",
            "extra": "mean: 976.0044790755667 nsec\nrounds: 102365"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17208.690457459415,
            "unit": "iter/sec",
            "range": "stddev: 0.000002463874299221873",
            "extra": "mean: 58.11017418623693 usec\nrounds: 17045"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 172475.59153565444,
            "unit": "iter/sec",
            "range": "stddev: 4.4947241514404344e-7",
            "extra": "mean: 5.797921845615345 usec\nrounds: 160746"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 1897.8747902426526,
            "unit": "iter/sec",
            "range": "stddev: 0.00000832114731435748",
            "extra": "mean: 526.9051494551678 usec\nrounds: 1927"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 183170.1184651376,
            "unit": "iter/sec",
            "range": "stddev: 4.2108169703697624e-7",
            "extra": "mean: 5.459405761045724 usec\nrounds: 129283"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2008.5211260044612,
            "unit": "iter/sec",
            "range": "stddev: 0.00003168637214727059",
            "extra": "mean: 497.8787561917728 usec\nrounds: 2059"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 184753.13860192202,
            "unit": "iter/sec",
            "range": "stddev: 6.729553482340741e-7",
            "extra": "mean: 5.412627940002946 usec\nrounds: 169177"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2093.4266604748136,
            "unit": "iter/sec",
            "range": "stddev: 0.000008468166868582705",
            "extra": "mean: 477.6857096934021 usec\nrounds: 2084"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 218868.3849481636,
            "unit": "iter/sec",
            "range": "stddev: 3.9386703906506555e-7",
            "extra": "mean: 4.568955905791685 usec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2479.3842312805778,
            "unit": "iter/sec",
            "range": "stddev: 0.000005253600843943129",
            "extra": "mean: 403.325949799846 usec\nrounds: 2490"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 10585.69429174694,
            "unit": "iter/sec",
            "range": "stddev: 0.0000024014238014338658",
            "extra": "mean: 94.46711499874341 usec\nrounds: 10661"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 167.38361736128408,
            "unit": "iter/sec",
            "range": "stddev: 0.00009659563942455403",
            "extra": "mean: 5.974300327382579 msec\nrounds: 168"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 59382.92627097604,
            "unit": "iter/sec",
            "range": "stddev: 8.317591647535257e-7",
            "extra": "mean: 16.8398572249 usec\nrounds: 53868"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 659.7247588724316,
            "unit": "iter/sec",
            "range": "stddev: 0.000008160245649826526",
            "extra": "mean: 1.515783645454128 msec\nrounds: 660"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "8008856ed8e653956a4df2555f3a53bb03db7cde",
          "message": "src: add APyFixedArray::_set_values_from_numpy_ndarray()",
          "timestamp": "2024-03-16T09:46:15+01:00",
          "tree_id": "d5bddbe20db7813749d1e5930f993d30f862cca9",
          "url": "https://github.com/apytypes/apytypes/commit/8008856ed8e653956a4df2555f3a53bb03db7cde"
        },
        "date": 1710579657433,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25119.71169972699,
            "unit": "iter/sec",
            "range": "stddev: 0.000001735920555371683",
            "extra": "mean: 39.809374086521395 usec\nrounds: 24227"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 108906.24960787271,
            "unit": "iter/sec",
            "range": "stddev: 7.132719145623845e-7",
            "extra": "mean: 9.182209502214931 usec\nrounds: 102481"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 934.0442346062429,
            "unit": "iter/sec",
            "range": "stddev: 0.0000196384180176387",
            "extra": "mean: 1.070613106906614 msec\nrounds: 1057"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 909799.1951415739,
            "unit": "iter/sec",
            "range": "stddev: 2.360941820049104e-7",
            "extra": "mean: 1.0991436410804776 usec\nrounds: 96164"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17241.519533030645,
            "unit": "iter/sec",
            "range": "stddev: 0.0000029284179145916485",
            "extra": "mean: 57.99952829472125 usec\nrounds: 16222"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 177821.42073969968,
            "unit": "iter/sec",
            "range": "stddev: 4.410743086377009e-7",
            "extra": "mean: 5.623619448321864 usec\nrounds: 165536"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 1993.5656130585708,
            "unit": "iter/sec",
            "range": "stddev: 0.000006733874616244417",
            "extra": "mean: 501.6137886055221 usec\nrounds: 2001"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 189327.46823471453,
            "unit": "iter/sec",
            "range": "stddev: 4.570614242069014e-7",
            "extra": "mean: 5.281853760175318 usec\nrounds: 131510"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2129.43738196274,
            "unit": "iter/sec",
            "range": "stddev: 0.000008598163462224604",
            "extra": "mean: 469.60761019339407 usec\nrounds: 2178"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 194369.5462198687,
            "unit": "iter/sec",
            "range": "stddev: 5.716474941187498e-7",
            "extra": "mean: 5.144838887820477 usec\nrounds: 173612"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2211.505069971012,
            "unit": "iter/sec",
            "range": "stddev: 0.000006145151545329525",
            "extra": "mean: 452.1807404281049 usec\nrounds: 2246"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 218511.61436809393,
            "unit": "iter/sec",
            "range": "stddev: 4.2323423644123135e-7",
            "extra": "mean: 4.576415779508403 usec\nrounds: 194175"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2485.3502195542173,
            "unit": "iter/sec",
            "range": "stddev: 0.000005888663672413968",
            "extra": "mean: 402.3577812624589 usec\nrounds: 2487"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 164697.08998061018,
            "unit": "iter/sec",
            "range": "stddev: 5.318254149949822e-7",
            "extra": "mean: 6.071752695313136 usec\nrounds: 151930"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2040.771832797785,
            "unit": "iter/sec",
            "range": "stddev: 0.000004554404887418058",
            "extra": "mean: 490.0106831781658 usec\nrounds: 2039"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 60758.723201862726,
            "unit": "iter/sec",
            "range": "stddev: 0.0000010539777980816594",
            "extra": "mean: 16.45854203811416 usec\nrounds: 54843"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 680.3165465658803,
            "unit": "iter/sec",
            "range": "stddev: 0.00005681581877008117",
            "extra": "mean: 1.469903980798095 msec\nrounds: 677"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "aebf5da2c6a4ff498bfc3075979a335665fe7a71",
          "message": "Add vector benchmarks",
          "timestamp": "2024-03-16T15:25:43+01:00",
          "tree_id": "c4eff7cfa1d4d610d35954ae510adbbe66247a20",
          "url": "https://github.com/apytypes/apytypes/commit/aebf5da2c6a4ff498bfc3075979a335665fe7a71"
        },
        "date": 1710599359955,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24889.677366047647,
            "unit": "iter/sec",
            "range": "stddev: 0.0000012080077876827624",
            "extra": "mean: 40.17729861633779 usec\nrounds: 23994"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 110254.59089212138,
            "unit": "iter/sec",
            "range": "stddev: 5.430354019001463e-7",
            "extra": "mean: 9.06991710647632 usec\nrounds: 106293"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 940.4603444015536,
            "unit": "iter/sec",
            "range": "stddev: 0.00001704728085363737",
            "extra": "mean: 1.0633090549249407 msec\nrounds: 1056"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 966930.3725008031,
            "unit": "iter/sec",
            "range": "stddev: 6.868296983742996e-8",
            "extra": "mean: 1.03420062957961 usec\nrounds: 96256"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17072.580852133877,
            "unit": "iter/sec",
            "range": "stddev: 0.000005024250912013255",
            "extra": "mean: 58.57345228943587 usec\nrounds: 16160"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 178424.70227308903,
            "unit": "iter/sec",
            "range": "stddev: 4.1248243576596474e-7",
            "extra": "mean: 5.6046051205927965 usec\nrounds: 162285"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 1994.788260563507,
            "unit": "iter/sec",
            "range": "stddev: 0.000007410416920849813",
            "extra": "mean: 501.306339008387 usec\nrounds: 1997"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 188411.97830814624,
            "unit": "iter/sec",
            "range": "stddev: 4.0786861725431346e-7",
            "extra": "mean: 5.30751817893716 usec\nrounds: 130976"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2139.868188667508,
            "unit": "iter/sec",
            "range": "stddev: 0.000008316321068725303",
            "extra": "mean: 467.3185036797514 usec\nrounds: 2174"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 195001.84375553075,
            "unit": "iter/sec",
            "range": "stddev: 3.9222662359996767e-7",
            "extra": "mean: 5.128156640681186 usec\nrounds: 169177"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2213.8391708141553,
            "unit": "iter/sec",
            "range": "stddev: 0.000006748395269974893",
            "extra": "mean: 451.70399601893513 usec\nrounds: 2261"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 216446.7502646316,
            "unit": "iter/sec",
            "range": "stddev: 4.207264572885727e-7",
            "extra": "mean: 4.620073984836374 usec\nrounds: 196851"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2478.8368162495553,
            "unit": "iter/sec",
            "range": "stddev: 0.000017963945223369424",
            "extra": "mean: 403.41501846538887 usec\nrounds: 2491"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 161664.2651095548,
            "unit": "iter/sec",
            "range": "stddev: 7.410094080461023e-7",
            "extra": "mean: 6.185658898225475 usec\nrounds: 138639"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2033.3407815373198,
            "unit": "iter/sec",
            "range": "stddev: 0.000017308589012883102",
            "extra": "mean: 491.80147719456244 usec\nrounds: 2039"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 60699.77232674723,
            "unit": "iter/sec",
            "range": "stddev: 7.836886916195965e-7",
            "extra": "mean: 16.47452637247129 usec\nrounds: 52953"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 682.3676365096688,
            "unit": "iter/sec",
            "range": "stddev: 0.000010735236004348456",
            "extra": "mean: 1.4654856802925627 msec\nrounds: 685"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 138302.71270501715,
            "unit": "iter/sec",
            "range": "stddev: 5.68321937568559e-7",
            "extra": "mean: 7.230516165889516 usec\nrounds: 97953"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 159047.4168393383,
            "unit": "iter/sec",
            "range": "stddev: 4.908886217774239e-7",
            "extra": "mean: 6.28743314335089 usec\nrounds: 127960"
          }
        ]
      }
    ],
    "APyTypes Floating-Point Array Benchmarks": [
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "02d2bd13ef7faaaef4c7acc1bc700225c53f5d5a",
          "message": "Add scalar benchmarks",
          "timestamp": "2024-03-14T23:49:40+01:00",
          "tree_id": "704141f81cde6793c35ac5ca1d86be8798eaaf06",
          "url": "https://github.com/apytypes/apytypes/commit/02d2bd13ef7faaaef4c7acc1bc700225c53f5d5a"
        },
        "date": 1710456782380,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 178.42619501305663,
            "unit": "iter/sec",
            "range": "stddev: 0.00011342755877080439",
            "extra": "mean: 5.604558231636467 msec\nrounds: 177"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5498.349938820842,
            "unit": "iter/sec",
            "range": "stddev: 0.0000059048359958663995",
            "extra": "mean: 181.87274566493974 usec\nrounds: 5363"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 54.898658145636674,
            "unit": "iter/sec",
            "range": "stddev: 0.0005182603756743615",
            "extra": "mean: 18.215381464282284 msec\nrounds: 56"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 8950.087810985546,
            "unit": "iter/sec",
            "range": "stddev: 0.000004690496819053579",
            "extra": "mean: 111.73074735340325 usec\nrounds: 8692"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 91.08099948458728,
            "unit": "iter/sec",
            "range": "stddev: 0.00020218417124651282",
            "extra": "mean: 10.979238322579231 msec\nrounds: 93"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6614.708679620969,
            "unit": "iter/sec",
            "range": "stddev: 0.000005822672316290902",
            "extra": "mean: 151.17823753612402 usec\nrounds: 6479"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 64.88692279249025,
            "unit": "iter/sec",
            "range": "stddev: 0.0006575570058977274",
            "extra": "mean: 15.411425861541025 msec\nrounds: 65"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 9657.452810125696,
            "unit": "iter/sec",
            "range": "stddev: 0.000005063315078935365",
            "extra": "mean: 103.54697244303537 usec\nrounds: 9435"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 95.73963770431051,
            "unit": "iter/sec",
            "range": "stddev: 0.0002907515838635902",
            "extra": "mean: 10.444994612246969 msec\nrounds: 98"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9902.22414172874,
            "unit": "iter/sec",
            "range": "stddev: 0.000004616995059747706",
            "extra": "mean: 100.98741309903524 usec\nrounds: 10138"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 141.5547218391259,
            "unit": "iter/sec",
            "range": "stddev: 0.0001846766382246028",
            "extra": "mean: 7.064405814286293 msec\nrounds: 140"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "8cb26a418e61f82a8962ffb3999e960269b37c07",
          "message": "Fix stupid error",
          "timestamp": "2024-03-14T23:54:22+01:00",
          "tree_id": "76800c16ba9dd5c629c6f3da76296ba2b5917c02",
          "url": "https://github.com/apytypes/apytypes/commit/8cb26a418e61f82a8962ffb3999e960269b37c07"
        },
        "date": 1710457062773,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 175.64513570145036,
            "unit": "iter/sec",
            "range": "stddev: 0.000021179910245709517",
            "extra": "mean: 5.693297431815772 msec\nrounds: 176"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5535.591593736885,
            "unit": "iter/sec",
            "range": "stddev: 0.000002935206171273214",
            "extra": "mean: 180.64916514640035 usec\nrounds: 5480"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 54.322628492782485,
            "unit": "iter/sec",
            "range": "stddev: 0.000034886518757894325",
            "extra": "mean: 18.408534854547103 msec\nrounds: 55"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 8854.177001572927,
            "unit": "iter/sec",
            "range": "stddev: 0.000003615617775081741",
            "extra": "mean: 112.94104464168177 usec\nrounds: 8893"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 89.03249873172402,
            "unit": "iter/sec",
            "range": "stddev: 0.0000766506253919218",
            "extra": "mean: 11.23185369662865 msec\nrounds: 89"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6314.43364164039,
            "unit": "iter/sec",
            "range": "stddev: 0.0000028262589933877757",
            "extra": "mean: 158.3673305877383 usec\nrounds: 6313"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 65.59568329517566,
            "unit": "iter/sec",
            "range": "stddev: 0.00019555462087512827",
            "extra": "mean: 15.244905606060614 msec\nrounds: 66"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 9316.673637524054,
            "unit": "iter/sec",
            "range": "stddev: 0.000004609491809259049",
            "extra": "mean: 107.33444563007727 usec\nrounds: 9463"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 95.31001865547792,
            "unit": "iter/sec",
            "range": "stddev: 0.0001437517960832334",
            "extra": "mean: 10.4920764270832 msec\nrounds: 96"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9659.33955360459,
            "unit": "iter/sec",
            "range": "stddev: 0.000008018014466487634",
            "extra": "mean: 103.52674677709497 usec\nrounds: 9541"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 141.7582531300485,
            "unit": "iter/sec",
            "range": "stddev: 0.00013962490990267304",
            "extra": "mean: 7.054263000000456 msec\nrounds: 137"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "d338d962f5c8961c92014489e294def34e6bb6b3",
          "message": "Remove failing benchmarks",
          "timestamp": "2024-03-15T00:01:02+01:00",
          "tree_id": "2daf9ddefd16aa714a5dd19fbc21a74046065b2e",
          "url": "https://github.com/apytypes/apytypes/commit/d338d962f5c8961c92014489e294def34e6bb6b3"
        },
        "date": 1710457460565,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 164.99273001273718,
            "unit": "iter/sec",
            "range": "stddev: 0.000024347652477583704",
            "extra": "mean: 6.060873105880493 msec\nrounds: 170"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5479.90398671206,
            "unit": "iter/sec",
            "range": "stddev: 0.000003671158036929942",
            "extra": "mean: 182.4849490839345 usec\nrounds: 5185"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 53.906963580470126,
            "unit": "iter/sec",
            "range": "stddev: 0.0001765127842728498",
            "extra": "mean: 18.55047907692372 msec\nrounds: 52"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 8774.766515309553,
            "unit": "iter/sec",
            "range": "stddev: 0.0000052732404578392355",
            "extra": "mean: 113.96314628489262 usec\nrounds: 8586"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 82.85701503134062,
            "unit": "iter/sec",
            "range": "stddev: 0.0025282561814518358",
            "extra": "mean: 12.068984136367337 msec\nrounds: 88"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6310.425360590106,
            "unit": "iter/sec",
            "range": "stddev: 0.000006848850379647174",
            "extra": "mean: 158.4679229779349 usec\nrounds: 6219"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 64.53739494963145,
            "unit": "iter/sec",
            "range": "stddev: 0.000040144733383672585",
            "extra": "mean: 15.494892546878525 msec\nrounds: 64"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 9309.257037662674,
            "unit": "iter/sec",
            "range": "stddev: 0.000004611831798800034",
            "extra": "mean: 107.4199580003299 usec\nrounds: 9262"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 94.70717583015013,
            "unit": "iter/sec",
            "range": "stddev: 0.000044961143013314894",
            "extra": "mean: 10.55886199999693 msec\nrounds: 96"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9601.047357218184,
            "unit": "iter/sec",
            "range": "stddev: 0.00000896619695758003",
            "extra": "mean: 104.15530335325218 usec\nrounds: 9573"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 139.32839816778764,
            "unit": "iter/sec",
            "range": "stddev: 0.00019742472862980149",
            "extra": "mean: 7.177287711265724 msec\nrounds: 142"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "70df93b98047c99b62eb6d4a322f9a3adc5001be",
          "message": "Add more conversion benchmarks",
          "timestamp": "2024-03-15T00:34:48+01:00",
          "tree_id": "3b5bc3581009c06b1f47c5d6037abbdc4bcfe5e2",
          "url": "https://github.com/apytypes/apytypes/commit/70df93b98047c99b62eb6d4a322f9a3adc5001be"
        },
        "date": 1710459490596,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 172.12533478368755,
            "unit": "iter/sec",
            "range": "stddev: 0.00014568592087082425",
            "extra": "mean: 5.809720000002991 msec\nrounds: 175"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5367.955584654394,
            "unit": "iter/sec",
            "range": "stddev: 0.000003275354295259083",
            "extra": "mean: 186.29066210211258 usec\nrounds: 5185"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 54.261059971174525,
            "unit": "iter/sec",
            "range": "stddev: 0.00003320854408766515",
            "extra": "mean: 18.429422509092834 msec\nrounds: 55"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 8738.644635343522,
            "unit": "iter/sec",
            "range": "stddev: 0.00001286500103276628",
            "extra": "mean: 114.43422197940073 usec\nrounds: 7789"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 88.64231852658367,
            "unit": "iter/sec",
            "range": "stddev: 0.00004141924888295166",
            "extra": "mean: 11.28129336666777 msec\nrounds: 90"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6346.530178817562,
            "unit": "iter/sec",
            "range": "stddev: 0.000007618637918462064",
            "extra": "mean: 157.56641374489018 usec\nrounds: 6417"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 62.658503262254676,
            "unit": "iter/sec",
            "range": "stddev: 0.00003571930190827692",
            "extra": "mean: 15.959525809522448 msec\nrounds: 63"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 9401.10397483568,
            "unit": "iter/sec",
            "range": "stddev: 0.000004577386467820841",
            "extra": "mean: 106.37048613404777 usec\nrounds: 9123"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 94.76046525405002,
            "unit": "iter/sec",
            "range": "stddev: 0.00003392525038950731",
            "extra": "mean: 10.552924126311849 msec\nrounds: 95"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9649.464776195584,
            "unit": "iter/sec",
            "range": "stddev: 0.00000462476634763043",
            "extra": "mean: 103.63269084798522 usec\nrounds: 9539"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 142.1528027018139,
            "unit": "iter/sec",
            "range": "stddev: 0.00007482743402910612",
            "extra": "mean: 7.034683671328274 msec\nrounds: 143"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73892.77502010431,
            "unit": "iter/sec",
            "range": "stddev: 6.055428005226598e-7",
            "extra": "mean: 13.533122821925769 usec\nrounds: 66055"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 835.4696861112163,
            "unit": "iter/sec",
            "range": "stddev: 0.000018821447058289397",
            "extra": "mean: 1.1969315184307974 msec\nrounds: 841"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "82bfe149914ec2681af1b24d7ae7885c16e1f954",
          "message": "APyFixed: specialize arithmetic when result fits single limb",
          "timestamp": "2024-03-15T09:25:20+01:00",
          "tree_id": "d6d91ca93a58a9965802be1f5c31bdf7e249a5ae",
          "url": "https://github.com/apytypes/apytypes/commit/82bfe149914ec2681af1b24d7ae7885c16e1f954"
        },
        "date": 1710492085269,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 202.95922405816324,
            "unit": "iter/sec",
            "range": "stddev: 0.00004274733147413569",
            "extra": "mean: 4.927098064355153 msec\nrounds: 202"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5798.773375667185,
            "unit": "iter/sec",
            "range": "stddev: 0.000004736873062026489",
            "extra": "mean: 172.45026408450454 usec\nrounds: 5680"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 57.493242316152106,
            "unit": "iter/sec",
            "range": "stddev: 0.00011259713306584317",
            "extra": "mean: 17.39334850000381 msec\nrounds: 58"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11632.793068925052,
            "unit": "iter/sec",
            "range": "stddev: 0.000002386197073998781",
            "extra": "mean: 85.96387764098745 usec\nrounds: 11548"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 117.42804696933453,
            "unit": "iter/sec",
            "range": "stddev: 0.00006229372217572967",
            "extra": "mean: 8.515853118643305 msec\nrounds: 118"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 7004.698321654101,
            "unit": "iter/sec",
            "range": "stddev: 0.000003643499114475612",
            "extra": "mean: 142.76132305493186 usec\nrounds: 7005"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 68.24018167611258,
            "unit": "iter/sec",
            "range": "stddev: 0.00006971389578543667",
            "extra": "mean: 14.654122768111696 msec\nrounds: 69"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 13172.761211971138,
            "unit": "iter/sec",
            "range": "stddev: 0.0000024301534247955017",
            "extra": "mean: 75.91422814916135 usec\nrounds: 13123"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 129.24040117373974,
            "unit": "iter/sec",
            "range": "stddev: 0.00012791785538040332",
            "extra": "mean: 7.737518538461403 msec\nrounds: 130"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9385.177818375523,
            "unit": "iter/sec",
            "range": "stddev: 0.0000028100957303931444",
            "extra": "mean: 106.55099129204241 usec\nrounds: 9187"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 136.05112972154157,
            "unit": "iter/sec",
            "range": "stddev: 0.00019469897184289924",
            "extra": "mean: 7.3501778489213505 msec\nrounds: 139"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 74004.05521880605,
            "unit": "iter/sec",
            "range": "stddev: 6.736868320994244e-7",
            "extra": "mean: 13.512773010118478 usec\nrounds: 66721"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 836.4090628687362,
            "unit": "iter/sec",
            "range": "stddev: 0.000008551937185988316",
            "extra": "mean: 1.1955872364297149 msec\nrounds: 829"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "f329bbd75bc608b8770b1d73155b9bbae3635fa0",
          "message": "APyFixedArray: return APyFixed when accessing vector APyFixedArrays",
          "timestamp": "2024-03-15T10:03:41+01:00",
          "tree_id": "7ed451bf7ff99dc7e810a030aa9439781ce81db0",
          "url": "https://github.com/apytypes/apytypes/commit/f329bbd75bc608b8770b1d73155b9bbae3635fa0"
        },
        "date": 1710495272855,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 206.79303619794211,
            "unit": "iter/sec",
            "range": "stddev: 0.000014047534950249311",
            "extra": "mean: 4.8357527815530545 msec\nrounds: 206"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5888.889741698571,
            "unit": "iter/sec",
            "range": "stddev: 0.000007767622898228042",
            "extra": "mean: 169.8112961631989 usec\nrounds: 5838"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 58.89907713812405,
            "unit": "iter/sec",
            "range": "stddev: 0.000046797229350806196",
            "extra": "mean: 16.978194711861157 msec\nrounds: 59"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11995.964722876251,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021003690920456986",
            "extra": "mean: 83.36136551760647 usec\nrounds: 11838"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 120.51857877508823,
            "unit": "iter/sec",
            "range": "stddev: 0.000021664044308378616",
            "extra": "mean: 8.297475876032358 msec\nrounds: 121"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 7138.571393087984,
            "unit": "iter/sec",
            "range": "stddev: 0.000002913838798063727",
            "extra": "mean: 140.084051126569 usec\nrounds: 7100"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 71.70561139342894,
            "unit": "iter/sec",
            "range": "stddev: 0.00009926653336883107",
            "extra": "mean: 13.945909958333324 msec\nrounds: 72"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 13010.699981282796,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021540932111816474",
            "extra": "mean: 76.85981549329405 usec\nrounds: 12883"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 130.21822964957767,
            "unit": "iter/sec",
            "range": "stddev: 0.00003037599508705153",
            "extra": "mean: 7.679416335877388 msec\nrounds: 131"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9588.106382913085,
            "unit": "iter/sec",
            "range": "stddev: 0.0000025752784501729155",
            "extra": "mean: 104.29588075723636 usec\nrounds: 9510"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 139.44951967431538,
            "unit": "iter/sec",
            "range": "stddev: 0.00004683782293448083",
            "extra": "mean: 7.17105374285621 msec\nrounds: 140"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73934.68444378696,
            "unit": "iter/sec",
            "range": "stddev: 5.820359624924187e-7",
            "extra": "mean: 13.52545165402453 usec\nrounds: 65838"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 835.2972783541115,
            "unit": "iter/sec",
            "range": "stddev: 0.000026636884051943942",
            "extra": "mean: 1.1971785685336154 msec\nrounds: 839"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "760013dc6684d9980a24c347ceebbd0c16fce71d",
          "message": "APyFloatArray: return APyFloat when accessing vector APyFloatArrays",
          "timestamp": "2024-03-15T10:29:16+01:00",
          "tree_id": "82e23fc0decab95cce932582050670ccb1005077",
          "url": "https://github.com/apytypes/apytypes/commit/760013dc6684d9980a24c347ceebbd0c16fce71d"
        },
        "date": 1710496009362,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 203.41332525474039,
            "unit": "iter/sec",
            "range": "stddev: 0.0001202473045058506",
            "extra": "mean: 4.916098779407254 msec\nrounds: 204"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5763.44727175542,
            "unit": "iter/sec",
            "range": "stddev: 0.0000027209175502404455",
            "extra": "mean: 173.50726966838752 usec\nrounds: 5529"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 58.264508282480385,
            "unit": "iter/sec",
            "range": "stddev: 0.000039134633559014105",
            "extra": "mean: 17.163107172410324 msec\nrounds: 58"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11798.736861852718,
            "unit": "iter/sec",
            "range": "stddev: 0.000002219031821054148",
            "extra": "mean: 84.75483534455003 usec\nrounds: 9863"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 118.20839665929405,
            "unit": "iter/sec",
            "range": "stddev: 0.0001777496480400401",
            "extra": "mean: 8.459635933327547 msec\nrounds: 120"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6995.066397222603,
            "unit": "iter/sec",
            "range": "stddev: 0.000002902610707964071",
            "extra": "mean: 142.95789964153178 usec\nrounds: 6965"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 68.5184582366188,
            "unit": "iter/sec",
            "range": "stddev: 0.000059135534977870775",
            "extra": "mean: 14.594607434782635 msec\nrounds: 69"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 13003.861413732195,
            "unit": "iter/sec",
            "range": "stddev: 0.0000035441638084886947",
            "extra": "mean: 76.90023510585793 usec\nrounds: 12505"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 133.3196874542693,
            "unit": "iter/sec",
            "range": "stddev: 0.000057723498391547815",
            "extra": "mean: 7.500767659262743 msec\nrounds: 135"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9738.874094836525,
            "unit": "iter/sec",
            "range": "stddev: 0.0000042961919245294765",
            "extra": "mean: 102.68127406331213 usec\nrounds: 9527"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 140.4758678786657,
            "unit": "iter/sec",
            "range": "stddev: 0.0001597363295615129",
            "extra": "mean: 7.1186604154938395 msec\nrounds: 142"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 74023.20028566984,
            "unit": "iter/sec",
            "range": "stddev: 7.166418388309744e-7",
            "extra": "mean: 13.509278120113784 usec\nrounds: 65540"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 841.8771190759059,
            "unit": "iter/sec",
            "range": "stddev: 0.000019193429686250987",
            "extra": "mean: 1.1878218059871484 msec\nrounds: 835"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "e7e2ccbf4ad2099fb3453ebb475036a26a8d0dc4",
          "message": "APyFixed::APyFixed(int, int, _IT, _IT): support short vector initialization",
          "timestamp": "2024-03-15T14:45:15+01:00",
          "tree_id": "70370597be0b422ef6e0f71486598e54c8c677b4",
          "url": "https://github.com/apytypes/apytypes/commit/e7e2ccbf4ad2099fb3453ebb475036a26a8d0dc4"
        },
        "date": 1710510820021,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 200.7626320410143,
            "unit": "iter/sec",
            "range": "stddev: 0.00010484747028850998",
            "extra": "mean: 4.981006623761076 msec\nrounds: 202"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5537.825503466687,
            "unit": "iter/sec",
            "range": "stddev: 0.000025209437723382638",
            "extra": "mean: 180.57629287416125 usec\nrounds: 5726"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 57.85260677317176,
            "unit": "iter/sec",
            "range": "stddev: 0.00022925618795007808",
            "extra": "mean: 17.28530581034655 msec\nrounds: 58"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11310.506647363007,
            "unit": "iter/sec",
            "range": "stddev: 0.000002239643710599814",
            "extra": "mean: 88.41336919537069 usec\nrounds: 11284"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 114.62299922438402,
            "unit": "iter/sec",
            "range": "stddev: 0.00004249312377726153",
            "extra": "mean: 8.724252608696943 msec\nrounds: 115"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6573.738296976394,
            "unit": "iter/sec",
            "range": "stddev: 0.000003424085001689563",
            "extra": "mean: 152.1204457530584 usec\nrounds: 6876"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 65.5737543515299,
            "unit": "iter/sec",
            "range": "stddev: 0.00005302479925880049",
            "extra": "mean: 15.250003753623254 msec\nrounds: 69"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12277.860113224464,
            "unit": "iter/sec",
            "range": "stddev: 0.000002233343983664524",
            "extra": "mean: 81.44741761008513 usec\nrounds: 12368"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 125.96811243001471,
            "unit": "iter/sec",
            "range": "stddev: 0.00014868622765999313",
            "extra": "mean: 7.938516984253291 msec\nrounds: 127"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9696.210846050526,
            "unit": "iter/sec",
            "range": "stddev: 0.000002720506034941979",
            "extra": "mean: 103.13307083326488 usec\nrounds: 9600"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 143.06379915818445,
            "unit": "iter/sec",
            "range": "stddev: 0.0000757924634903384",
            "extra": "mean: 6.989888468530801 msec\nrounds: 143"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73878.47377530858,
            "unit": "iter/sec",
            "range": "stddev: 6.774448281880581e-7",
            "extra": "mean: 13.53574253633562 usec\nrounds: 65885"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 838.4307379991558,
            "unit": "iter/sec",
            "range": "stddev: 0.000007644739520608874",
            "extra": "mean: 1.1927043638528994 msec\nrounds: 841"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "1a2af23f9e038c24d1f056f8a80e56302548c759",
          "message": "src: python_sequence_extract_shape, filter strings",
          "timestamp": "2024-03-15T15:21:37+01:00",
          "tree_id": "e1147f625d2fed813b4f8f2268c7fe02924d5ccf",
          "url": "https://github.com/apytypes/apytypes/commit/1a2af23f9e038c24d1f056f8a80e56302548c759"
        },
        "date": 1710514099310,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 203.3844552398008,
            "unit": "iter/sec",
            "range": "stddev: 0.0001699386979330698",
            "extra": "mean: 4.9167966097553935 msec\nrounds: 205"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5775.303194790268,
            "unit": "iter/sec",
            "range": "stddev: 0.000006514645430800769",
            "extra": "mean: 173.15108250975825 usec\nrounds: 5769"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 57.641115520587164,
            "unit": "iter/sec",
            "range": "stddev: 0.000052177285761635484",
            "extra": "mean: 17.34872739655496 msec\nrounds: 58"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11697.741645379907,
            "unit": "iter/sec",
            "range": "stddev: 0.0000029180608544544782",
            "extra": "mean: 85.48658624162348 usec\nrounds: 10001"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 117.7129361728855,
            "unit": "iter/sec",
            "range": "stddev: 0.0001363980229054992",
            "extra": "mean: 8.495243025212586 msec\nrounds: 119"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6867.742414203908,
            "unit": "iter/sec",
            "range": "stddev: 0.00000371212382829846",
            "extra": "mean: 145.60825664221093 usec\nrounds: 6850"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 71.00706449285221,
            "unit": "iter/sec",
            "range": "stddev: 0.00003852776752120413",
            "extra": "mean: 14.083105774646734 msec\nrounds: 71"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12728.919994590717,
            "unit": "iter/sec",
            "range": "stddev: 0.000003596548155157536",
            "extra": "mean: 78.56126053309787 usec\nrounds: 12532"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 126.41652528611213,
            "unit": "iter/sec",
            "range": "stddev: 0.000026526169413734055",
            "extra": "mean: 7.910358220468017 msec\nrounds: 127"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9669.163384909845,
            "unit": "iter/sec",
            "range": "stddev: 0.0000029900687266314777",
            "extra": "mean: 103.42156401666016 usec\nrounds: 9560"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 139.3423000652363,
            "unit": "iter/sec",
            "range": "stddev: 0.0008260990790818236",
            "extra": "mean: 7.176571647890319 msec\nrounds: 142"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73914.41756192707,
            "unit": "iter/sec",
            "range": "stddev: 7.31421744957493e-7",
            "extra": "mean: 13.529160250260766 usec\nrounds: 65198"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 836.897054366268,
            "unit": "iter/sec",
            "range": "stddev: 0.000032124504139915286",
            "extra": "mean: 1.1948900940477563 msec\nrounds: 840"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "ba7fb25e8c9c94902e2b8bc6cdfe5fb2ba19546f",
          "message": "Add relative benchmark plot",
          "timestamp": "2024-03-15T17:43:34+01:00",
          "tree_id": "c2b7135118f18b3b808d84ad4c1dc5e488d8ee3f",
          "url": "https://github.com/apytypes/apytypes/commit/ba7fb25e8c9c94902e2b8bc6cdfe5fb2ba19546f"
        },
        "date": 1710521213616,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 204.30725152037792,
            "unit": "iter/sec",
            "range": "stddev: 0.0000772607779891127",
            "extra": "mean: 4.894588873172025 msec\nrounds: 205"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5770.242482452368,
            "unit": "iter/sec",
            "range": "stddev: 0.000007803704724143285",
            "extra": "mean: 173.3029423011349 usec\nrounds: 5754"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 58.42739626702882,
            "unit": "iter/sec",
            "range": "stddev: 0.00020342254110337863",
            "extra": "mean: 17.11525866101807 msec\nrounds: 59"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11730.74681192254,
            "unit": "iter/sec",
            "range": "stddev: 0.0000043003830010016585",
            "extra": "mean: 85.24606455435986 usec\nrounds: 11804"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 118.6731763321151,
            "unit": "iter/sec",
            "range": "stddev: 0.000026063156572480266",
            "extra": "mean: 8.42650404166676 msec\nrounds: 120"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6903.7357986723455,
            "unit": "iter/sec",
            "range": "stddev: 0.000004908258209667719",
            "extra": "mean: 144.84911201154446 usec\nrounds: 6910"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 69.29726852689807,
            "unit": "iter/sec",
            "range": "stddev: 0.000050671847948104286",
            "extra": "mean: 14.4305832142842 msec\nrounds: 70"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12638.159876523356,
            "unit": "iter/sec",
            "range": "stddev: 0.000003038214005327556",
            "extra": "mean: 79.1254430842895 usec\nrounds: 12659"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 128.58880771189214,
            "unit": "iter/sec",
            "range": "stddev: 0.00014493783980468877",
            "extra": "mean: 7.776726589148692 msec\nrounds: 129"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9786.04719451427,
            "unit": "iter/sec",
            "range": "stddev: 0.0000024774585201109942",
            "extra": "mean: 102.18630465634442 usec\nrounds: 9578"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 144.04135705641394,
            "unit": "iter/sec",
            "range": "stddev: 0.00005292421925926125",
            "extra": "mean: 6.942450560281441 msec\nrounds: 141"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73883.19774128693,
            "unit": "iter/sec",
            "range": "stddev: 8.075939877380364e-7",
            "extra": "mean: 13.534877083983961 usec\nrounds: 65622"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 838.6270199698324,
            "unit": "iter/sec",
            "range": "stddev: 0.00000536519046272082",
            "extra": "mean: 1.192425209523982 msec\nrounds: 840"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "fd93e3629cd4d50702706a40146f8d7ca4afc002",
          "message": "Special case short negations",
          "timestamp": "2024-03-15T18:35:57+01:00",
          "tree_id": "9f5f101b34999448b5e13a4ac81d084c5952c5ef",
          "url": "https://github.com/apytypes/apytypes/commit/fd93e3629cd4d50702706a40146f8d7ca4afc002"
        },
        "date": 1710524345528,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 199.64985046220522,
            "unit": "iter/sec",
            "range": "stddev: 0.00009677858261998531",
            "extra": "mean: 5.008769090910515 msec\nrounds: 209"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5658.772230793512,
            "unit": "iter/sec",
            "range": "stddev: 0.00002638636930195228",
            "extra": "mean: 176.71677869596337 usec\nrounds: 5689"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 57.26948233808815,
            "unit": "iter/sec",
            "range": "stddev: 0.00026700455288535885",
            "extra": "mean: 17.46130677585907 msec\nrounds: 58"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11391.393331366764,
            "unit": "iter/sec",
            "range": "stddev: 0.0000019957784922407093",
            "extra": "mean: 87.78557380214856 usec\nrounds: 11375"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 113.2978253679803,
            "unit": "iter/sec",
            "range": "stddev: 0.000236850715842115",
            "extra": "mean: 8.826294739127583 msec\nrounds: 115"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6730.831761854587,
            "unit": "iter/sec",
            "range": "stddev: 0.000002868858812463918",
            "extra": "mean: 148.5700483062533 usec\nrounds: 6728"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 69.4740028757537,
            "unit": "iter/sec",
            "range": "stddev: 0.00020620472552069943",
            "extra": "mean: 14.39387337143054 msec\nrounds: 70"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12355.92923197999,
            "unit": "iter/sec",
            "range": "stddev: 0.000004218051497563399",
            "extra": "mean: 80.93280409957106 usec\nrounds: 12343"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 126.46249966547566,
            "unit": "iter/sec",
            "range": "stddev: 0.000026084851583772103",
            "extra": "mean: 7.907482476190534 msec\nrounds: 126"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9497.152288833338,
            "unit": "iter/sec",
            "range": "stddev: 0.000007777060718790371",
            "extra": "mean: 105.29472094238086 usec\nrounds: 9335"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 138.92538312612572,
            "unit": "iter/sec",
            "range": "stddev: 0.00003831206308894331",
            "extra": "mean: 7.198108635713701 msec\nrounds: 140"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73771.086560342,
            "unit": "iter/sec",
            "range": "stddev: 7.109631993695145e-7",
            "extra": "mean: 13.555446267990607 usec\nrounds: 65929"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 843.0527493121116,
            "unit": "iter/sec",
            "range": "stddev: 0.000021608484099543252",
            "extra": "mean: 1.1861653980915778 msec\nrounds: 839"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "dadfb6c0a1245b2d954e945b0bf6cca7715fd61d",
          "message": "Special case short abs",
          "timestamp": "2024-03-15T18:55:40+01:00",
          "tree_id": "ee091c534560f949b05acda375c11f87d6d15196",
          "url": "https://github.com/apytypes/apytypes/commit/dadfb6c0a1245b2d954e945b0bf6cca7715fd61d"
        },
        "date": 1710525534066,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 202.9805185387605,
            "unit": "iter/sec",
            "range": "stddev: 0.000018570630058947754",
            "extra": "mean: 4.926581167487969 msec\nrounds: 203"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5811.36402405553,
            "unit": "iter/sec",
            "range": "stddev: 0.000006554754121833914",
            "extra": "mean: 172.07664084724433 usec\nrounds: 5758"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 57.610537785830005,
            "unit": "iter/sec",
            "range": "stddev: 0.0002524672517235408",
            "extra": "mean: 17.35793551724077 msec\nrounds: 58"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11403.080002566134,
            "unit": "iter/sec",
            "range": "stddev: 0.000010421659095947991",
            "extra": "mean: 87.69560502732256 usec\nrounds: 10940"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 115.79468251332533,
            "unit": "iter/sec",
            "range": "stddev: 0.0001252248162004028",
            "extra": "mean: 8.63597514406521 msec\nrounds: 118"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 7065.1649857397715,
            "unit": "iter/sec",
            "range": "stddev: 0.0000034518067706951654",
            "extra": "mean: 141.53951139405603 usec\nrounds: 7065"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 68.93700693287373,
            "unit": "iter/sec",
            "range": "stddev: 0.00015421702389864994",
            "extra": "mean: 14.505996771425446 msec\nrounds: 70"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12867.214068585428,
            "unit": "iter/sec",
            "range": "stddev: 0.000002365083917815355",
            "extra": "mean: 77.71690085124513 usec\nrounds: 12809"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 128.40700651336135,
            "unit": "iter/sec",
            "range": "stddev: 0.0001275646980008884",
            "extra": "mean: 7.787737033616972 msec\nrounds: 119"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9572.916212942224,
            "unit": "iter/sec",
            "range": "stddev: 0.0000035175613850106044",
            "extra": "mean: 104.46137600661724 usec\nrounds: 9561"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 141.3990535980059,
            "unit": "iter/sec",
            "range": "stddev: 0.00008593891889097281",
            "extra": "mean: 7.07218311971858 msec\nrounds: 142"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73939.41568573374,
            "unit": "iter/sec",
            "range": "stddev: 7.251742771988453e-7",
            "extra": "mean: 13.524586186213874 usec\nrounds: 66455"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 838.1189042454499,
            "unit": "iter/sec",
            "range": "stddev: 0.00000780202157720888",
            "extra": "mean: 1.1931481260410062 msec\nrounds: 841"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "8008856ed8e653956a4df2555f3a53bb03db7cde",
          "message": "src: add APyFixedArray::_set_values_from_numpy_ndarray()",
          "timestamp": "2024-03-16T09:46:15+01:00",
          "tree_id": "d5bddbe20db7813749d1e5930f993d30f862cca9",
          "url": "https://github.com/apytypes/apytypes/commit/8008856ed8e653956a4df2555f3a53bb03db7cde"
        },
        "date": 1710579675223,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 203.30613914787185,
            "unit": "iter/sec",
            "range": "stddev: 0.000038013270249718806",
            "extra": "mean: 4.9186906218934405 msec\nrounds: 201"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5877.0959081738665,
            "unit": "iter/sec",
            "range": "stddev: 0.0000032895933979044443",
            "extra": "mean: 170.15206415284115 usec\nrounds: 5861"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 57.26820216683289,
            "unit": "iter/sec",
            "range": "stddev: 0.0001188314407545014",
            "extra": "mean: 17.461697105259468 msec\nrounds: 57"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11685.89015903005,
            "unit": "iter/sec",
            "range": "stddev: 0.0000028482472460860824",
            "extra": "mean: 85.57328422493077 usec\nrounds: 11607"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 117.46457520511444,
            "unit": "iter/sec",
            "range": "stddev: 0.000055358174767814194",
            "extra": "mean: 8.513204923729718 msec\nrounds: 118"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6744.223508485732,
            "unit": "iter/sec",
            "range": "stddev: 0.000011353483579456053",
            "extra": "mean: 148.2750384446449 usec\nrounds: 6789"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 68.420622855061,
            "unit": "iter/sec",
            "range": "stddev: 0.00006351031223065552",
            "extra": "mean: 14.615476420294398 msec\nrounds: 69"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12872.432452421921,
            "unit": "iter/sec",
            "range": "stddev: 0.0000033982638746926148",
            "extra": "mean: 77.68539502507564 usec\nrounds: 12865"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 126.7245997811513,
            "unit": "iter/sec",
            "range": "stddev: 0.00009657020391043885",
            "extra": "mean: 7.891127703121281 msec\nrounds: 128"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9699.36550408515,
            "unit": "iter/sec",
            "range": "stddev: 0.0000039648260897068225",
            "extra": "mean: 103.0995274462874 usec\nrounds: 9546"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 141.9504714456436,
            "unit": "iter/sec",
            "range": "stddev: 0.00006475246089203627",
            "extra": "mean: 7.044710664331433 msec\nrounds: 143"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73704.29565261005,
            "unit": "iter/sec",
            "range": "stddev: 6.677681643016962e-7",
            "extra": "mean: 13.567730227194533 usec\nrounds: 66278"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 836.6419594250381,
            "unit": "iter/sec",
            "range": "stddev: 0.000018779384966039025",
            "extra": "mean: 1.1952544200475266 msec\nrounds: 838"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "aebf5da2c6a4ff498bfc3075979a335665fe7a71",
          "message": "Add vector benchmarks",
          "timestamp": "2024-03-16T15:25:43+01:00",
          "tree_id": "c4eff7cfa1d4d610d35954ae510adbbe66247a20",
          "url": "https://github.com/apytypes/apytypes/commit/aebf5da2c6a4ff498bfc3075979a335665fe7a71"
        },
        "date": 1710599378350,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 203.33059501906044,
            "unit": "iter/sec",
            "range": "stddev: 0.000044542083284966836",
            "extra": "mean: 4.91809901951184 msec\nrounds: 205"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5741.538455778332,
            "unit": "iter/sec",
            "range": "stddev: 0.000004882499752505775",
            "extra": "mean: 174.16934636980298 usec\nrounds: 5702"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 57.47136516479477,
            "unit": "iter/sec",
            "range": "stddev: 0.00005172638970563",
            "extra": "mean: 17.39996948276026 msec\nrounds: 58"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11692.233385374464,
            "unit": "iter/sec",
            "range": "stddev: 0.0000038032248722208794",
            "extra": "mean: 85.52685932961928 usec\nrounds: 11694"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 115.93780111467862,
            "unit": "iter/sec",
            "range": "stddev: 0.0005138989849669102",
            "extra": "mean: 8.625314525422652 msec\nrounds: 118"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6851.729992972157,
            "unit": "iter/sec",
            "range": "stddev: 0.0000030923867643904124",
            "extra": "mean: 145.9485416129508 usec\nrounds: 6957"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 67.08412019282463,
            "unit": "iter/sec",
            "range": "stddev: 0.00014883822844011046",
            "extra": "mean: 14.90665744926861 msec\nrounds: 69"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12524.37260525634,
            "unit": "iter/sec",
            "range": "stddev: 0.0000027322938638806927",
            "extra": "mean: 79.84431887472839 usec\nrounds: 12933"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 129.79434711531164,
            "unit": "iter/sec",
            "range": "stddev: 0.0001302956733211207",
            "extra": "mean: 7.704495782944861 msec\nrounds: 129"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9767.320718752664,
            "unit": "iter/sec",
            "range": "stddev: 0.000003114209706909701",
            "extra": "mean: 102.38222218709994 usec\nrounds: 9447"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 142.13909603511857,
            "unit": "iter/sec",
            "range": "stddev: 0.00006102660495054541",
            "extra": "mean: 7.035362035459463 msec\nrounds: 141"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73950.13919057614,
            "unit": "iter/sec",
            "range": "stddev: 8.198484469386651e-7",
            "extra": "mean: 13.522624986856487 usec\nrounds: 66499"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 837.5914016889766,
            "unit": "iter/sec",
            "range": "stddev: 0.000010626028770678088",
            "extra": "mean: 1.1938995529127108 msec\nrounds: 841"
          }
        ]
      }
    ],
    "APyTypes Fixed-Point Scalar Benchmarks": [
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "8cb26a418e61f82a8962ffb3999e960269b37c07",
          "message": "Fix stupid error",
          "timestamp": "2024-03-14T23:54:22+01:00",
          "tree_id": "76800c16ba9dd5c629c6f3da76296ba2b5917c02",
          "url": "https://github.com/apytypes/apytypes/commit/8cb26a418e61f82a8962ffb3999e960269b37c07"
        },
        "date": 1710457083536,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 3074334.1929871812,
            "unit": "iter/sec",
            "range": "stddev: 2.4450351894778527e-8",
            "extra": "mean: 325.2736811375567 nsec\nrounds: 193424"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2557404.5491606207,
            "unit": "iter/sec",
            "range": "stddev: 2.9706487919124535e-8",
            "extra": "mean: 391.0214362949629 nsec\nrounds: 191939"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3009559.989522354,
            "unit": "iter/sec",
            "range": "stddev: 2.724435419290272e-8",
            "extra": "mean: 332.2744864636465 nsec\nrounds: 195734"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3086635.9914819566,
            "unit": "iter/sec",
            "range": "stddev: 2.722454260713887e-8",
            "extra": "mean: 323.97730174845776 nsec\nrounds: 188324"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2224384.9005145268,
            "unit": "iter/sec",
            "range": "stddev: 3.216203841063232e-8",
            "extra": "mean: 449.5624834392148 nsec\nrounds: 186220"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2719184.4528788426,
            "unit": "iter/sec",
            "range": "stddev: 3.077018031022221e-8",
            "extra": "mean: 367.7573247895174 nsec\nrounds: 191205"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3618048.8438697103,
            "unit": "iter/sec",
            "range": "stddev: 2.434091291901924e-8",
            "extra": "mean: 276.39206742436136 nsec\nrounds: 197278"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3614916.2360352767,
            "unit": "iter/sec",
            "range": "stddev: 2.2984929943289064e-8",
            "extra": "mean: 276.6315827821624 nsec\nrounds: 196890"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3357081.973883308,
            "unit": "iter/sec",
            "range": "stddev: 2.4631354569232697e-8",
            "extra": "mean: 297.87774256915793 nsec\nrounds: 194970"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "d338d962f5c8961c92014489e294def34e6bb6b3",
          "message": "Remove failing benchmarks",
          "timestamp": "2024-03-15T00:01:02+01:00",
          "tree_id": "2daf9ddefd16aa714a5dd19fbc21a74046065b2e",
          "url": "https://github.com/apytypes/apytypes/commit/d338d962f5c8961c92014489e294def34e6bb6b3"
        },
        "date": 1710457480858,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 3093529.8981867866,
            "unit": "iter/sec",
            "range": "stddev: 2.8778004686099555e-8",
            "extra": "mean: 323.25532091559535 nsec\nrounds: 196890"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2684823.592214238,
            "unit": "iter/sec",
            "range": "stddev: 3.320030739975409e-8",
            "extra": "mean: 372.46394992208445 nsec\nrounds: 187301"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3018708.811647864,
            "unit": "iter/sec",
            "range": "stddev: 3.413389486661534e-8",
            "extra": "mean: 331.26745982966014 nsec\nrounds: 190115"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3191877.6456509903,
            "unit": "iter/sec",
            "range": "stddev: 2.9276187449287752e-8",
            "extra": "mean: 313.2952171153941 nsec\nrounds: 193837"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2295034.3072956144,
            "unit": "iter/sec",
            "range": "stddev: 3.267497805734877e-8",
            "extra": "mean: 435.723333991612 nsec\nrounds: 189036"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2485775.800719556,
            "unit": "iter/sec",
            "range": "stddev: 3.353348768642525e-8",
            "extra": "mean: 402.2888949640124 nsec\nrounds: 190477"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3481070.086216322,
            "unit": "iter/sec",
            "range": "stddev: 2.5381025215840316e-8",
            "extra": "mean: 287.2679880705969 nsec\nrounds: 196079"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3679294.769322513,
            "unit": "iter/sec",
            "range": "stddev: 2.148801629274691e-8",
            "extra": "mean: 271.7912161694704 nsec\nrounds: 187935"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3393350.308095964,
            "unit": "iter/sec",
            "range": "stddev: 2.6928867465567927e-8",
            "extra": "mean: 294.694007162846 nsec\nrounds: 198060"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "70df93b98047c99b62eb6d4a322f9a3adc5001be",
          "message": "Add more conversion benchmarks",
          "timestamp": "2024-03-15T00:34:48+01:00",
          "tree_id": "3b5bc3581009c06b1f47c5d6037abbdc4bcfe5e2",
          "url": "https://github.com/apytypes/apytypes/commit/70df93b98047c99b62eb6d4a322f9a3adc5001be"
        },
        "date": 1710459514737,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 3036558.618436206,
            "unit": "iter/sec",
            "range": "stddev: 2.513429253484953e-8",
            "extra": "mean: 329.32016985563376 nsec\nrounds: 189394"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2708989.429412127,
            "unit": "iter/sec",
            "range": "stddev: 3.029040808943124e-8",
            "extra": "mean: 369.14134442266925 nsec\nrounds: 189394"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2977724.969129932,
            "unit": "iter/sec",
            "range": "stddev: 2.4787846150483957e-8",
            "extra": "mean: 335.82685115879906 nsec\nrounds: 195351"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2881917.144423283,
            "unit": "iter/sec",
            "range": "stddev: 2.6591880318191555e-8",
            "extra": "mean: 346.9912387783116 nsec\nrounds: 188680"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2243440.333871009,
            "unit": "iter/sec",
            "range": "stddev: 3.7850647998735896e-8",
            "extra": "mean: 445.7439696087305 nsec\nrounds: 188324"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2745114.4669787395,
            "unit": "iter/sec",
            "range": "stddev: 2.931696293308794e-8",
            "extra": "mean: 364.2835342675565 nsec\nrounds: 195313"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3482062.7943767933,
            "unit": "iter/sec",
            "range": "stddev: 2.7095423717458854e-8",
            "extra": "mean: 287.18609027238585 nsec\nrounds: 191571"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3660642.4488111367,
            "unit": "iter/sec",
            "range": "stddev: 2.0528750649733253e-8",
            "extra": "mean: 273.1760924436968 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3421362.7731697327,
            "unit": "iter/sec",
            "range": "stddev: 2.418950821244635e-8",
            "extra": "mean: 292.28119503786303 nsec\nrounds: 198453"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5073997.467632342,
            "unit": "iter/sec",
            "range": "stddev: 1.2855027775044095e-8",
            "extra": "mean: 197.08326745904884 nsec\nrounds: 51107"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4853238.691202506,
            "unit": "iter/sec",
            "range": "stddev: 3.599937501902349e-8",
            "extra": "mean: 206.04797406988484 nsec\nrounds: 50056"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7572159.343365915,
            "unit": "iter/sec",
            "range": "stddev: 1.4483555491832854e-8",
            "extra": "mean: 132.06272539365358 nsec\nrounds: 77376"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "82bfe149914ec2681af1b24d7ae7885c16e1f954",
          "message": "APyFixed: specialize arithmetic when result fits single limb",
          "timestamp": "2024-03-15T09:25:20+01:00",
          "tree_id": "d6d91ca93a58a9965802be1f5c31bdf7e249a5ae",
          "url": "https://github.com/apytypes/apytypes/commit/82bfe149914ec2681af1b24d7ae7885c16e1f954"
        },
        "date": 1710492109412,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 3968942.9955858532,
            "unit": "iter/sec",
            "range": "stddev: 2.8214044881833244e-8",
            "extra": "mean: 251.9562516045416 nsec\nrounds: 198413"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2510045.1691008774,
            "unit": "iter/sec",
            "range": "stddev: 4.0612855696991964e-8",
            "extra": "mean: 398.39920504626633 nsec\nrounds: 170011"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2956680.5591262584,
            "unit": "iter/sec",
            "range": "stddev: 2.9640343334061254e-8",
            "extra": "mean: 338.21712559150035 nsec\nrounds: 191608"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2859780.4197502765,
            "unit": "iter/sec",
            "range": "stddev: 3.215808218400075e-8",
            "extra": "mean: 349.6771965756562 nsec\nrounds: 187618"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2206729.197322361,
            "unit": "iter/sec",
            "range": "stddev: 4.4581247097477495e-8",
            "extra": "mean: 453.15936419076024 nsec\nrounds: 198847"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2466507.3847053945,
            "unit": "iter/sec",
            "range": "stddev: 3.4945297522547574e-8",
            "extra": "mean: 405.43158565055893 nsec\nrounds: 187970"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3842264.956265691,
            "unit": "iter/sec",
            "range": "stddev: 2.4117290379611464e-8",
            "extra": "mean: 260.2631550355648 nsec\nrounds: 193462"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3613709.8890985576,
            "unit": "iter/sec",
            "range": "stddev: 2.6168490967920755e-8",
            "extra": "mean: 276.72392933829985 nsec\nrounds: 198060"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3414129.0609384673,
            "unit": "iter/sec",
            "range": "stddev: 2.601515465755358e-8",
            "extra": "mean: 292.9004680699974 nsec\nrounds: 198060"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4931557.988601426,
            "unit": "iter/sec",
            "range": "stddev: 9.132839880807878e-9",
            "extra": "mean: 202.7756750121304 nsec\nrounds: 50925"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4665009.184229996,
            "unit": "iter/sec",
            "range": "stddev: 1.010801239603113e-8",
            "extra": "mean: 214.3618502146527 nsec\nrounds: 47081"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7688291.261339424,
            "unit": "iter/sec",
            "range": "stddev: 7.071187202552529e-9",
            "extra": "mean: 130.06791314326816 nsec\nrounds: 76541"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "f329bbd75bc608b8770b1d73155b9bbae3635fa0",
          "message": "APyFixedArray: return APyFixed when accessing vector APyFixedArrays",
          "timestamp": "2024-03-15T10:03:41+01:00",
          "tree_id": "7ed451bf7ff99dc7e810a030aa9439781ce81db0",
          "url": "https://github.com/apytypes/apytypes/commit/f329bbd75bc608b8770b1d73155b9bbae3635fa0"
        },
        "date": 1710495296924,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4159316.343508309,
            "unit": "iter/sec",
            "range": "stddev: 1.9362526520190144e-8",
            "extra": "mean: 240.4241268065435 nsec\nrounds: 194553"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2680019.2924030693,
            "unit": "iter/sec",
            "range": "stddev: 2.990702195323462e-8",
            "extra": "mean: 373.1316423111945 nsec\nrounds: 190513"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2974578.4630993297,
            "unit": "iter/sec",
            "range": "stddev: 2.8356017319680317e-8",
            "extra": "mean: 336.18208845572553 nsec\nrounds: 194932"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2801902.7519426653,
            "unit": "iter/sec",
            "range": "stddev: 2.8432735502946123e-8",
            "extra": "mean: 356.9003240054486 nsec\nrounds: 199204"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2338609.6019693133,
            "unit": "iter/sec",
            "range": "stddev: 3.407387930329288e-8",
            "extra": "mean: 427.6045044704754 nsec\nrounds: 190877"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2525104.5412370977,
            "unit": "iter/sec",
            "range": "stddev: 3.141167110761116e-8",
            "extra": "mean: 396.0232076213521 nsec\nrounds: 189394"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3974050.547319583,
            "unit": "iter/sec",
            "range": "stddev: 1.9312271392206544e-8",
            "extra": "mean: 251.63243096503678 nsec\nrounds: 198847"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3624782.7955312547,
            "unit": "iter/sec",
            "range": "stddev: 2.1334301790585865e-8",
            "extra": "mean: 275.8785991900341 nsec\nrounds: 196464"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3370165.139013984,
            "unit": "iter/sec",
            "range": "stddev: 2.993171516399283e-8",
            "extra": "mean: 296.72136490388016 nsec\nrounds: 194553"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5678474.071943722,
            "unit": "iter/sec",
            "range": "stddev: 8.759930075327445e-9",
            "extra": "mean: 176.10364815093357 nsec\nrounds: 56456"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 5253659.212221404,
            "unit": "iter/sec",
            "range": "stddev: 9.14372119389458e-9",
            "extra": "mean: 190.34352241077212 nsec\nrounds: 52095"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8750889.581342177,
            "unit": "iter/sec",
            "range": "stddev: 6.3688073200983254e-9",
            "extra": "mean: 114.27409644518166 nsec\nrounds: 78346"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "760013dc6684d9980a24c347ceebbd0c16fce71d",
          "message": "APyFloatArray: return APyFloat when accessing vector APyFloatArrays",
          "timestamp": "2024-03-15T10:29:16+01:00",
          "tree_id": "82e23fc0decab95cce932582050670ccb1005077",
          "url": "https://github.com/apytypes/apytypes/commit/760013dc6684d9980a24c347ceebbd0c16fce71d"
        },
        "date": 1710496034149,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 3953678.1806407967,
            "unit": "iter/sec",
            "range": "stddev: 1.9618336575359953e-8",
            "extra": "mean: 252.9290332470996 nsec\nrounds: 186220"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2558066.3745095055,
            "unit": "iter/sec",
            "range": "stddev: 4.0317621933366965e-8",
            "extra": "mean: 390.9202708595394 nsec\nrounds: 198060"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2980240.4613060984,
            "unit": "iter/sec",
            "range": "stddev: 2.6907559317453222e-8",
            "extra": "mean: 335.5433942272928 nsec\nrounds: 195695"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3020072.661512323,
            "unit": "iter/sec",
            "range": "stddev: 2.8468161341477457e-8",
            "extra": "mean: 331.11786108462604 nsec\nrounds: 197278"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2313572.6084062746,
            "unit": "iter/sec",
            "range": "stddev: 3.5142836601080936e-8",
            "extra": "mean: 432.2319500008567 nsec\nrounds: 191205"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2456413.9994060593,
            "unit": "iter/sec",
            "range": "stddev: 3.586892773468309e-8",
            "extra": "mean: 407.097500764135 nsec\nrounds: 189036"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3915910.7164355274,
            "unit": "iter/sec",
            "range": "stddev: 2.1599990870420716e-8",
            "extra": "mean: 255.36843723292992 nsec\nrounds: 197668"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3582627.437788362,
            "unit": "iter/sec",
            "range": "stddev: 2.4296024090987653e-8",
            "extra": "mean: 279.1247533730315 nsec\nrounds: 198847"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3416202.7218390857,
            "unit": "iter/sec",
            "range": "stddev: 2.311035901940952e-8",
            "extra": "mean: 292.72267526968255 nsec\nrounds: 194175"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4501233.758114604,
            "unit": "iter/sec",
            "range": "stddev: 1.263643186454912e-8",
            "extra": "mean: 222.16131259499056 nsec\nrounds: 49025"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4998736.604652031,
            "unit": "iter/sec",
            "range": "stddev: 1.1667901424700937e-8",
            "extra": "mean: 200.05054858647026 nsec\nrounds: 49983"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8955877.482036732,
            "unit": "iter/sec",
            "range": "stddev: 5.9316774401300184e-9",
            "extra": "mean: 111.65851721460885 nsec\nrounds: 88091"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "e7e2ccbf4ad2099fb3453ebb475036a26a8d0dc4",
          "message": "APyFixed::APyFixed(int, int, _IT, _IT): support short vector initialization",
          "timestamp": "2024-03-15T14:45:15+01:00",
          "tree_id": "70370597be0b422ef6e0f71486598e54c8c677b4",
          "url": "https://github.com/apytypes/apytypes/commit/e7e2ccbf4ad2099fb3453ebb475036a26a8d0dc4"
        },
        "date": 1710510843824,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4040927.4833014733,
            "unit": "iter/sec",
            "range": "stddev: 2.299808525899719e-8",
            "extra": "mean: 247.46794000444712 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2650466.040412742,
            "unit": "iter/sec",
            "range": "stddev: 3.4602460330950035e-8",
            "extra": "mean: 377.2921383457062 nsec\nrounds: 106180"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2910442.5972797447,
            "unit": "iter/sec",
            "range": "stddev: 2.763399212571151e-8",
            "extra": "mean: 343.59035321112475 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2786418.2606849503,
            "unit": "iter/sec",
            "range": "stddev: 3.017950595565615e-8",
            "extra": "mean: 358.88366585500677 nsec\nrounds: 195313"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2134192.909709529,
            "unit": "iter/sec",
            "range": "stddev: 3.659564756426329e-8",
            "extra": "mean: 468.56120430842884 nsec\nrounds: 189036"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2404341.0367458714,
            "unit": "iter/sec",
            "range": "stddev: 3.406810811745502e-8",
            "extra": "mean: 415.91437517259664 nsec\nrounds: 196851"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4209609.516990883,
            "unit": "iter/sec",
            "range": "stddev: 2.0706514706213983e-8",
            "extra": "mean: 237.55172444475576 nsec\nrounds: 197278"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3733408.842605716,
            "unit": "iter/sec",
            "range": "stddev: 2.210227101922283e-8",
            "extra": "mean: 267.8517253691127 nsec\nrounds: 193088"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3491386.942096654,
            "unit": "iter/sec",
            "range": "stddev: 2.6526289882015608e-8",
            "extra": "mean: 286.419127007304 nsec\nrounds: 190477"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4376705.670530965,
            "unit": "iter/sec",
            "range": "stddev: 1.0519256100336031e-8",
            "extra": "mean: 228.48235071714893 nsec\nrounds: 43893"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4070380.917108727,
            "unit": "iter/sec",
            "range": "stddev: 1.9658630885583374e-8",
            "extra": "mean: 245.6772524155541 nsec\nrounds: 198020"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8332933.31078711,
            "unit": "iter/sec",
            "range": "stddev: 7.394063866092451e-9",
            "extra": "mean: 120.00576060118286 nsec\nrounds: 79917"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "1a2af23f9e038c24d1f056f8a80e56302548c759",
          "message": "src: python_sequence_extract_shape, filter strings",
          "timestamp": "2024-03-15T15:21:37+01:00",
          "tree_id": "e1147f625d2fed813b4f8f2268c7fe02924d5ccf",
          "url": "https://github.com/apytypes/apytypes/commit/1a2af23f9e038c24d1f056f8a80e56302548c759"
        },
        "date": 1710514123661,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4128176.7255355874,
            "unit": "iter/sec",
            "range": "stddev: 2.0144663636133917e-8",
            "extra": "mean: 242.2376914763952 nsec\nrounds: 198020"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2649587.7093963814,
            "unit": "iter/sec",
            "range": "stddev: 3.14903050741767e-8",
            "extra": "mean: 377.417209648757 nsec\nrounds: 198808"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2972561.262480376,
            "unit": "iter/sec",
            "range": "stddev: 1.4157604799363374e-7",
            "extra": "mean: 336.41022394464915 nsec\nrounds: 198847"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3075389.030227679,
            "unit": "iter/sec",
            "range": "stddev: 2.782967606156212e-8",
            "extra": "mean: 325.16211450684904 nsec\nrounds: 189754"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2183828.774612571,
            "unit": "iter/sec",
            "range": "stddev: 3.9344947256846885e-8",
            "extra": "mean: 457.91135808121874 nsec\nrounds: 198847"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2624766.0078642797,
            "unit": "iter/sec",
            "range": "stddev: 3.404178817151513e-8",
            "extra": "mean: 380.98634202199366 nsec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4178596.039497362,
            "unit": "iter/sec",
            "range": "stddev: 4.808625390080146e-8",
            "extra": "mean: 239.3148298010857 nsec\nrounds: 196503"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3804433.2651883904,
            "unit": "iter/sec",
            "range": "stddev: 2.1802077113702315e-8",
            "extra": "mean: 262.8512396707047 nsec\nrounds: 196890"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3569235.6887664185,
            "unit": "iter/sec",
            "range": "stddev: 2.394176188165755e-8",
            "extra": "mean: 280.172027626737 nsec\nrounds: 196503"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4511842.287466333,
            "unit": "iter/sec",
            "range": "stddev: 1.5469311475286996e-8",
            "extra": "mean: 221.6389528458655 nsec\nrounds: 46211"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4463615.081591347,
            "unit": "iter/sec",
            "range": "stddev: 1.0559531558099654e-8",
            "extra": "mean: 224.03365472173218 nsec\nrounds: 44619"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8118848.852411865,
            "unit": "iter/sec",
            "range": "stddev: 8.233463393590443e-9",
            "extra": "mean: 123.17017081839923 nsec\nrounds: 82427"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "ba7fb25e8c9c94902e2b8bc6cdfe5fb2ba19546f",
          "message": "Add relative benchmark plot",
          "timestamp": "2024-03-15T17:43:34+01:00",
          "tree_id": "c2b7135118f18b3b808d84ad4c1dc5e488d8ee3f",
          "url": "https://github.com/apytypes/apytypes/commit/ba7fb25e8c9c94902e2b8bc6cdfe5fb2ba19546f"
        },
        "date": 1710521236716,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4185830.349103962,
            "unit": "iter/sec",
            "range": "stddev: 1.7804225545177425e-8",
            "extra": "mean: 238.9012254675082 nsec\nrounds: 197239"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2632460.921269035,
            "unit": "iter/sec",
            "range": "stddev: 3.304294765512879e-8",
            "extra": "mean: 379.8726856382655 nsec\nrounds: 98532"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3083400.661989292,
            "unit": "iter/sec",
            "range": "stddev: 2.8994835565552152e-8",
            "extra": "mean: 324.317242428961 nsec\nrounds: 191608"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2775897.495672739,
            "unit": "iter/sec",
            "range": "stddev: 3.2717498125033645e-8",
            "extra": "mean: 360.24384962301286 nsec\nrounds: 194175"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2303025.240408865,
            "unit": "iter/sec",
            "range": "stddev: 3.650110963841764e-8",
            "extra": "mean: 434.2114808183655 nsec\nrounds: 190840"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2609502.128612864,
            "unit": "iter/sec",
            "range": "stddev: 3.0622756106486696e-8",
            "extra": "mean: 383.2148627261333 nsec\nrounds: 198453"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4154350.3101718505,
            "unit": "iter/sec",
            "range": "stddev: 2.107924009939216e-8",
            "extra": "mean: 240.71152535026502 nsec\nrounds: 198020"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3735107.367427557,
            "unit": "iter/sec",
            "range": "stddev: 2.0926834307283773e-8",
            "extra": "mean: 267.7299208906572 nsec\nrounds: 196079"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3572095.790853551,
            "unit": "iter/sec",
            "range": "stddev: 2.4737700850073302e-8",
            "extra": "mean: 279.9476997676177 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4674193.931773874,
            "unit": "iter/sec",
            "range": "stddev: 1.1384519874501535e-8",
            "extra": "mean: 213.94063117536487 nsec\nrounds: 46754"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4414890.686606187,
            "unit": "iter/sec",
            "range": "stddev: 9.477023289007423e-9",
            "extra": "mean: 226.50617444137504 nsec\nrounds: 44600"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8021875.406694794,
            "unit": "iter/sec",
            "range": "stddev: 8.074530523913405e-9",
            "extra": "mean: 124.65912885731254 nsec\nrounds: 80756"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "fd93e3629cd4d50702706a40146f8d7ca4afc002",
          "message": "Special case short negations",
          "timestamp": "2024-03-15T18:35:57+01:00",
          "tree_id": "9f5f101b34999448b5e13a4ac81d084c5952c5ef",
          "url": "https://github.com/apytypes/apytypes/commit/fd93e3629cd4d50702706a40146f8d7ca4afc002"
        },
        "date": 1710524369479,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4099480.8882106654,
            "unit": "iter/sec",
            "range": "stddev: 2.5901394051823217e-8",
            "extra": "mean: 243.93332406447274 nsec\nrounds: 196117"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2748588.346018135,
            "unit": "iter/sec",
            "range": "stddev: 2.9050522256763777e-8",
            "extra": "mean: 363.8231244954118 nsec\nrounds: 196079"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2907765.09641331,
            "unit": "iter/sec",
            "range": "stddev: 4.8210662716731075e-8",
            "extra": "mean: 343.90673484382086 nsec\nrounds: 190477"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2995407.3500102824,
            "unit": "iter/sec",
            "range": "stddev: 2.7798636816589067e-8",
            "extra": "mean: 333.8444101756684 nsec\nrounds: 194553"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2275314.118052583,
            "unit": "iter/sec",
            "range": "stddev: 3.2411001317544415e-8",
            "extra": "mean: 439.49975612856974 nsec\nrounds: 186916"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2455487.791104253,
            "unit": "iter/sec",
            "range": "stddev: 3.175447153897235e-8",
            "extra": "mean: 407.2510576607802 nsec\nrounds: 186916"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3883879.6258991193,
            "unit": "iter/sec",
            "range": "stddev: 1.99350801905879e-8",
            "extra": "mean: 257.4745090789381 nsec\nrounds: 194175"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3469000.091614046,
            "unit": "iter/sec",
            "range": "stddev: 2.25667421399734e-8",
            "extra": "mean: 288.2675046384521 nsec\nrounds: 196851"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3359735.865484051,
            "unit": "iter/sec",
            "range": "stddev: 2.4153723961499355e-8",
            "extra": "mean: 297.6424457273207 nsec\nrounds: 189036"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4807630.927410077,
            "unit": "iter/sec",
            "range": "stddev: 1.275646531635389e-8",
            "extra": "mean: 208.0026555904767 nsec\nrounds: 48268"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4556623.831445007,
            "unit": "iter/sec",
            "range": "stddev: 1.2726082191772629e-8",
            "extra": "mean: 219.46073167135455 nsec\nrounds: 46018"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7918736.604259769,
            "unit": "iter/sec",
            "range": "stddev: 8.331629318339291e-9",
            "extra": "mean: 126.2827708478109 nsec\nrounds: 78716"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "dadfb6c0a1245b2d954e945b0bf6cca7715fd61d",
          "message": "Special case short abs",
          "timestamp": "2024-03-15T18:55:40+01:00",
          "tree_id": "ee091c534560f949b05acda375c11f87d6d15196",
          "url": "https://github.com/apytypes/apytypes/commit/dadfb6c0a1245b2d954e945b0bf6cca7715fd61d"
        },
        "date": 1710525557605,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4362216.272184342,
            "unit": "iter/sec",
            "range": "stddev: 2.3693402209978215e-8",
            "extra": "mean: 229.2412704011288 nsec\nrounds: 44600"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2661532.1405609054,
            "unit": "iter/sec",
            "range": "stddev: 3.343397137539439e-8",
            "extra": "mean: 375.7234356708267 nsec\nrounds: 189790"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2830066.5810058727,
            "unit": "iter/sec",
            "range": "stddev: 5.9112373287432805e-8",
            "extra": "mean: 353.3485772778251 nsec\nrounds: 188324"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3185832.634449643,
            "unit": "iter/sec",
            "range": "stddev: 2.7402650587721753e-8",
            "extra": "mean: 313.8896843439333 nsec\nrounds: 198847"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2395675.9168788176,
            "unit": "iter/sec",
            "range": "stddev: 3.698804572183438e-8",
            "extra": "mean: 417.41873053631605 nsec\nrounds: 198453"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2721510.2879821677,
            "unit": "iter/sec",
            "range": "stddev: 3.263527587701512e-8",
            "extra": "mean: 367.44303500004804 nsec\nrounds: 190513"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3907468.019506126,
            "unit": "iter/sec",
            "range": "stddev: 2.2923229008385166e-8",
            "extra": "mean: 255.920200755948 nsec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3557828.0441394034,
            "unit": "iter/sec",
            "range": "stddev: 2.589449396622856e-8",
            "extra": "mean: 281.07035741866207 nsec\nrounds: 196503"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3268644.331304798,
            "unit": "iter/sec",
            "range": "stddev: 2.94690956418432e-8",
            "extra": "mean: 305.93723227159745 nsec\nrounds: 192345"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4876244.782991991,
            "unit": "iter/sec",
            "range": "stddev: 8.900835589977316e-9",
            "extra": "mean: 205.0758410422589 nsec\nrounds: 49611"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4592322.548396379,
            "unit": "iter/sec",
            "range": "stddev: 9.967794324452648e-9",
            "extra": "mean: 217.75473945080248 nsec\nrounds: 45807"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7582879.431983545,
            "unit": "iter/sec",
            "range": "stddev: 7.308984516358648e-9",
            "extra": "mean: 131.87602532386995 nsec\nrounds: 76371"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "8008856ed8e653956a4df2555f3a53bb03db7cde",
          "message": "src: add APyFixedArray::_set_values_from_numpy_ndarray()",
          "timestamp": "2024-03-16T09:46:15+01:00",
          "tree_id": "d5bddbe20db7813749d1e5930f993d30f862cca9",
          "url": "https://github.com/apytypes/apytypes/commit/8008856ed8e653956a4df2555f3a53bb03db7cde"
        },
        "date": 1710579698840,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4376355.321589005,
            "unit": "iter/sec",
            "range": "stddev: 1.1244965411495173e-8",
            "extra": "mean: 228.50064186222045 nsec\nrounds: 44402"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2687674.777848112,
            "unit": "iter/sec",
            "range": "stddev: 2.9936659316471665e-8",
            "extra": "mean: 372.0688262739296 nsec\nrounds: 189036"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3010661.456460599,
            "unit": "iter/sec",
            "range": "stddev: 2.896348016178263e-8",
            "extra": "mean: 332.15292202785355 nsec\nrounds: 196079"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2787872.394708688,
            "unit": "iter/sec",
            "range": "stddev: 3.038505845290944e-8",
            "extra": "mean: 358.69647473750337 nsec\nrounds: 198060"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2149279.613979543,
            "unit": "iter/sec",
            "range": "stddev: 3.7006123221104197e-8",
            "extra": "mean: 465.27217468408867 nsec\nrounds: 193799"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2485503.584116331,
            "unit": "iter/sec",
            "range": "stddev: 3.45706292585967e-8",
            "extra": "mean: 402.3329543318563 nsec\nrounds: 191939"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4260967.295737542,
            "unit": "iter/sec",
            "range": "stddev: 2.1575612879451105e-8",
            "extra": "mean: 234.68849455858893 nsec\nrounds: 191571"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3706976.5237304834,
            "unit": "iter/sec",
            "range": "stddev: 2.2524131182614267e-8",
            "extra": "mean: 269.7616220655671 nsec\nrounds: 191976"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3416886.677763322,
            "unit": "iter/sec",
            "range": "stddev: 3.6732181640294415e-8",
            "extra": "mean: 292.6640811670586 nsec\nrounds: 197239"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5263504.57271031,
            "unit": "iter/sec",
            "range": "stddev: 1.1395377793554409e-8",
            "extra": "mean: 189.98748574947192 nsec\nrounds: 51932"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 5126373.595662468,
            "unit": "iter/sec",
            "range": "stddev: 1.1237487320035457e-8",
            "extra": "mean: 195.0696689071131 nsec\nrounds: 51798"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8136831.625607189,
            "unit": "iter/sec",
            "range": "stddev: 8.575304311817691e-9",
            "extra": "mean: 122.89795905982348 nsec\nrounds: 82217"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "aebf5da2c6a4ff498bfc3075979a335665fe7a71",
          "message": "Add vector benchmarks",
          "timestamp": "2024-03-16T15:25:43+01:00",
          "tree_id": "c4eff7cfa1d4d610d35954ae510adbbe66247a20",
          "url": "https://github.com/apytypes/apytypes/commit/aebf5da2c6a4ff498bfc3075979a335665fe7a71"
        },
        "date": 1710599409461,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4423107.857442453,
            "unit": "iter/sec",
            "range": "stddev: 1.4245306270070267e-8",
            "extra": "mean: 226.08537531304057 nsec\nrounds: 44323"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2664754.8824238074,
            "unit": "iter/sec",
            "range": "stddev: 2.924407607507639e-8",
            "extra": "mean: 375.26903753728425 nsec\nrounds: 187618"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2849337.8812686284,
            "unit": "iter/sec",
            "range": "stddev: 2.9117399224940012e-8",
            "extra": "mean: 350.95872854327047 nsec\nrounds: 186568"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3080481.066126889,
            "unit": "iter/sec",
            "range": "stddev: 2.534390636829394e-8",
            "extra": "mean: 324.62462145800725 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2396256.2473435267,
            "unit": "iter/sec",
            "range": "stddev: 3.641287089004167e-8",
            "extra": "mean: 417.31763917508204 nsec\nrounds: 197239"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2558387.3082753494,
            "unit": "iter/sec",
            "range": "stddev: 5.216170094619566e-8",
            "extra": "mean: 390.8712323442997 nsec\nrounds: 196503"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4217722.860326209,
            "unit": "iter/sec",
            "range": "stddev: 1.7731148892201203e-8",
            "extra": "mean: 237.09476253327514 nsec\nrounds: 198060"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4006664.463409919,
            "unit": "iter/sec",
            "range": "stddev: 2.477753556616339e-8",
            "extra": "mean: 249.5841638681597 nsec\nrounds: 191571"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3737478.992533247,
            "unit": "iter/sec",
            "range": "stddev: 2.1579210048800667e-8",
            "extra": "mean: 267.56003230975597 nsec\nrounds: 198060"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3426274.255173409,
            "unit": "iter/sec",
            "range": "stddev: 2.570201557395168e-8",
            "extra": "mean: 291.8622169520597 nsec\nrounds: 191939"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5225011.421840929,
            "unit": "iter/sec",
            "range": "stddev: 9.199127527366198e-9",
            "extra": "mean: 191.38714143667272 nsec\nrounds: 52285"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4934142.863986462,
            "unit": "iter/sec",
            "range": "stddev: 1.1895443661477538e-8",
            "extra": "mean: 202.66944585221032 nsec\nrounds: 49734"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8190752.181331766,
            "unit": "iter/sec",
            "range": "stddev: 7.831825021357063e-9",
            "extra": "mean: 122.0889092798192 nsec\nrounds: 81680"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4870487.1774679255,
            "unit": "iter/sec",
            "range": "stddev: 1.1481758319425168e-8",
            "extra": "mean: 205.3182697259292 nsec\nrounds: 49784"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4146503.051038411,
            "unit": "iter/sec",
            "range": "stddev: 1.2443956394442579e-8",
            "extra": "mean: 241.16707203422632 nsec\nrounds: 46797"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5876823.52243662,
            "unit": "iter/sec",
            "range": "stddev: 1.1102322229759944e-8",
            "extra": "mean: 170.1599505553019 nsec\nrounds: 59663"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5327533.063809259,
            "unit": "iter/sec",
            "range": "stddev: 8.530186822738256e-9",
            "extra": "mean: 187.7041377356217 nsec\nrounds: 56456"
          }
        ]
      }
    ],
    "APyTypes Floating-Point Scalar Benchmarks": [
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "d338d962f5c8961c92014489e294def34e6bb6b3",
          "message": "Remove failing benchmarks",
          "timestamp": "2024-03-15T00:01:02+01:00",
          "tree_id": "2daf9ddefd16aa714a5dd19fbc21a74046065b2e",
          "url": "https://github.com/apytypes/apytypes/commit/d338d962f5c8961c92014489e294def34e6bb6b3"
        },
        "date": 1710457492763,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 1874851.0276935827,
            "unit": "iter/sec",
            "range": "stddev: 4.3617072756176794e-8",
            "extra": "mean: 533.3757110451431 nsec\nrounds: 184129"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1596700.4929751004,
            "unit": "iter/sec",
            "range": "stddev: 4.676451524345361e-8",
            "extra": "mean: 626.2915333211379 nsec\nrounds: 155232"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1321098.4846912536,
            "unit": "iter/sec",
            "range": "stddev: 5.193148545197236e-8",
            "extra": "mean: 756.9458383215524 nsec\nrounds: 130993"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1068439.2870689386,
            "unit": "iter/sec",
            "range": "stddev: 7.120802584034707e-8",
            "extra": "mean: 935.9446176331654 nsec\nrounds: 106180"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1372011.1407195495,
            "unit": "iter/sec",
            "range": "stddev: 5.555519239298547e-8",
            "extra": "mean: 728.8570553993954 nsec\nrounds: 135428"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "70df93b98047c99b62eb6d4a322f9a3adc5001be",
          "message": "Add more conversion benchmarks",
          "timestamp": "2024-03-15T00:34:48+01:00",
          "tree_id": "3b5bc3581009c06b1f47c5d6037abbdc4bcfe5e2",
          "url": "https://github.com/apytypes/apytypes/commit/70df93b98047c99b62eb6d4a322f9a3adc5001be"
        },
        "date": 1710459530544,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 1899727.1299955416,
            "unit": "iter/sec",
            "range": "stddev: 4.926576279356105e-8",
            "extra": "mean: 526.3913875895053 nsec\nrounds: 186568"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1533208.172197811,
            "unit": "iter/sec",
            "range": "stddev: 5.184479393903952e-8",
            "extra": "mean: 652.2271522766911 nsec\nrounds: 151700"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1516500.8176471323,
            "unit": "iter/sec",
            "range": "stddev: 5.711401977986179e-8",
            "extra": "mean: 659.4127667873155 nsec\nrounds: 149881"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1378529.3711860026,
            "unit": "iter/sec",
            "range": "stddev: 5.331970644779745e-8",
            "extra": "mean: 725.4107318291136 nsec\nrounds: 136370"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1268765.6586279438,
            "unit": "iter/sec",
            "range": "stddev: 6.989495898251052e-8",
            "extra": "mean: 788.1676125136041 nsec\nrounds: 126024"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5334102.061283643,
            "unit": "iter/sec",
            "range": "stddev: 7.537984156573323e-9",
            "extra": "mean: 187.4729783028541 nsec\nrounds: 52953"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5336126.713109632,
            "unit": "iter/sec",
            "range": "stddev: 7.316912365924438e-9",
            "extra": "mean: 187.40184665096433 nsec\nrounds: 54098"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10002620.658316232,
            "unit": "iter/sec",
            "range": "stddev: 5.02436090623904e-9",
            "extra": "mean: 99.97380028293287 nsec\nrounds: 100011"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "82bfe149914ec2681af1b24d7ae7885c16e1f954",
          "message": "APyFixed: specialize arithmetic when result fits single limb",
          "timestamp": "2024-03-15T09:25:20+01:00",
          "tree_id": "d6d91ca93a58a9965802be1f5c31bdf7e249a5ae",
          "url": "https://github.com/apytypes/apytypes/commit/82bfe149914ec2681af1b24d7ae7885c16e1f954"
        },
        "date": 1710492125862,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2272340.873436498,
            "unit": "iter/sec",
            "range": "stddev: 3.5901022656805546e-8",
            "extra": "mean: 440.07481962319935 nsec\nrounds: 190477"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1715539.9067785023,
            "unit": "iter/sec",
            "range": "stddev: 4.524914778561112e-8",
            "extra": "mean: 582.9068715036234 nsec\nrounds: 169177"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1506906.2732375881,
            "unit": "iter/sec",
            "range": "stddev: 4.9032249502188704e-8",
            "extra": "mean: 663.6112794536847 nsec\nrounds: 148766"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1495563.0864964856,
            "unit": "iter/sec",
            "range": "stddev: 5.141113009703485e-8",
            "extra": "mean: 668.6444784770455 nsec\nrounds: 147233"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1596695.8861994117,
            "unit": "iter/sec",
            "range": "stddev: 5.476231661803378e-8",
            "extra": "mean: 626.2933402930502 nsec\nrounds: 157184"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5719756.701701948,
            "unit": "iter/sec",
            "range": "stddev: 1.0799183893405897e-8",
            "extra": "mean: 174.83261127215445 nsec\nrounds: 58439"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5826583.44650573,
            "unit": "iter/sec",
            "range": "stddev: 9.282903015412752e-9",
            "extra": "mean: 171.62716524717126 nsec\nrounds: 57931"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10386992.20555985,
            "unit": "iter/sec",
            "range": "stddev: 6.538787938375285e-9",
            "extra": "mean: 96.27426113449272 nsec\nrounds: 102691"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "f329bbd75bc608b8770b1d73155b9bbae3635fa0",
          "message": "APyFixedArray: return APyFixed when accessing vector APyFixedArrays",
          "timestamp": "2024-03-15T10:03:41+01:00",
          "tree_id": "7ed451bf7ff99dc7e810a030aa9439781ce81db0",
          "url": "https://github.com/apytypes/apytypes/commit/f329bbd75bc608b8770b1d73155b9bbae3635fa0"
        },
        "date": 1710495313154,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2254614.219541764,
            "unit": "iter/sec",
            "range": "stddev: 3.681314966725069e-8",
            "extra": "mean: 443.53485901603955 nsec\nrounds: 192308"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1643520.674869282,
            "unit": "iter/sec",
            "range": "stddev: 5.093622471223612e-8",
            "extra": "mean: 608.4499059189212 nsec\nrounds: 165810"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1609526.2975732856,
            "unit": "iter/sec",
            "range": "stddev: 5.402845294180536e-8",
            "extra": "mean: 621.3008147227886 nsec\nrounds: 158705"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1273513.3232358857,
            "unit": "iter/sec",
            "range": "stddev: 6.423528118749464e-8",
            "extra": "mean: 785.2293193596604 nsec\nrounds: 126824"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1435501.5865598265,
            "unit": "iter/sec",
            "range": "stddev: 5.078627952665994e-8",
            "extra": "mean: 696.6206163495674 nsec\nrounds: 141184"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6950555.076380312,
            "unit": "iter/sec",
            "range": "stddev: 9.016148166473703e-9",
            "extra": "mean: 143.87340133429987 nsec\nrounds: 69411"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6691030.189750905,
            "unit": "iter/sec",
            "range": "stddev: 9.789266777345176e-9",
            "extra": "mean: 149.45381677273923 nsec\nrounds: 66944"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9911575.136131603,
            "unit": "iter/sec",
            "range": "stddev: 5.5892996299072014e-9",
            "extra": "mean: 100.89213735111305 nsec\nrounds: 100021"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "760013dc6684d9980a24c347ceebbd0c16fce71d",
          "message": "APyFloatArray: return APyFloat when accessing vector APyFloatArrays",
          "timestamp": "2024-03-15T10:29:16+01:00",
          "tree_id": "82e23fc0decab95cce932582050670ccb1005077",
          "url": "https://github.com/apytypes/apytypes/commit/760013dc6684d9980a24c347ceebbd0c16fce71d"
        },
        "date": 1710496050841,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2204062.2101480886,
            "unit": "iter/sec",
            "range": "stddev: 3.7895416641011496e-8",
            "extra": "mean: 453.7077018043846 nsec\nrounds: 197668"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1648221.1042672272,
            "unit": "iter/sec",
            "range": "stddev: 4.8794757485904434e-8",
            "extra": "mean: 606.7147164970931 nsec\nrounds: 161499"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1316373.2066986137,
            "unit": "iter/sec",
            "range": "stddev: 5.737914092287917e-8",
            "extra": "mean: 759.6629853230844 nsec\nrounds: 131510"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1489556.980701133,
            "unit": "iter/sec",
            "range": "stddev: 5.165295430763081e-8",
            "extra": "mean: 671.3405482006472 nsec\nrounds: 144655"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1449459.8828577893,
            "unit": "iter/sec",
            "range": "stddev: 5.1261764723905396e-8",
            "extra": "mean: 689.9121609549825 nsec\nrounds: 142390"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5756234.430444422,
            "unit": "iter/sec",
            "range": "stddev: 9.493195438167116e-9",
            "extra": "mean: 173.72468270424733 nsec\nrounds: 57202"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5346873.1651047785,
            "unit": "iter/sec",
            "range": "stddev: 1.0006944015538583e-8",
            "extra": "mean: 187.02519568380177 nsec\nrounds: 59663"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10103011.884909716,
            "unit": "iter/sec",
            "range": "stddev: 5.698474911604888e-9",
            "extra": "mean: 98.98038440339099 nsec\nrounds: 99921"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "e7e2ccbf4ad2099fb3453ebb475036a26a8d0dc4",
          "message": "APyFixed::APyFixed(int, int, _IT, _IT): support short vector initialization",
          "timestamp": "2024-03-15T14:45:15+01:00",
          "tree_id": "70370597be0b422ef6e0f71486598e54c8c677b4",
          "url": "https://github.com/apytypes/apytypes/commit/e7e2ccbf4ad2099fb3453ebb475036a26a8d0dc4"
        },
        "date": 1710510859925,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2179156.1067845686,
            "unit": "iter/sec",
            "range": "stddev: 4.1030018832775824e-8",
            "extra": "mean: 458.89323710522683 nsec\nrounds: 181160"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1684845.9325280215,
            "unit": "iter/sec",
            "range": "stddev: 4.684074426545991e-8",
            "extra": "mean: 593.5260789688189 nsec\nrounds: 162840"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1483919.3614853714,
            "unit": "iter/sec",
            "range": "stddev: 5.1145541139061306e-8",
            "extra": "mean: 673.8910657510497 nsec\nrounds: 145709"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1453040.4345529417,
            "unit": "iter/sec",
            "range": "stddev: 5.294909036417207e-8",
            "extra": "mean: 688.2120939102904 nsec\nrounds: 143411"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1438781.2875514762,
            "unit": "iter/sec",
            "range": "stddev: 5.710360702680683e-8",
            "extra": "mean: 695.0326701161604 nsec\nrounds: 141784"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6014952.07086147,
            "unit": "iter/sec",
            "range": "stddev: 1.1495254682105608e-8",
            "extra": "mean: 166.25236381261294 nsec\nrounds: 64231"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6374808.1724868575,
            "unit": "iter/sec",
            "range": "stddev: 8.200850687865086e-9",
            "extra": "mean: 156.86746533260896 nsec\nrounds: 60939"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10214980.458355034,
            "unit": "iter/sec",
            "range": "stddev: 6.5179810797152324e-9",
            "extra": "mean: 97.89543935759764 nsec\nrounds: 101751"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "1a2af23f9e038c24d1f056f8a80e56302548c759",
          "message": "src: python_sequence_extract_shape, filter strings",
          "timestamp": "2024-03-15T15:21:37+01:00",
          "tree_id": "e1147f625d2fed813b4f8f2268c7fe02924d5ccf",
          "url": "https://github.com/apytypes/apytypes/commit/1a2af23f9e038c24d1f056f8a80e56302548c759"
        },
        "date": 1710514139887,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2260130.5245116437,
            "unit": "iter/sec",
            "range": "stddev: 3.8507569580127116e-8",
            "extra": "mean: 442.45232262243064 nsec\nrounds: 187970"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1773894.0865431025,
            "unit": "iter/sec",
            "range": "stddev: 4.942325968538757e-8",
            "extra": "mean: 563.7315145172616 nsec\nrounds: 175132"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1191314.4425690942,
            "unit": "iter/sec",
            "range": "stddev: 6.115186078253734e-8",
            "extra": "mean: 839.4089455034666 nsec\nrounds: 117981"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1478943.161826334,
            "unit": "iter/sec",
            "range": "stddev: 5.767783046506377e-8",
            "extra": "mean: 676.15850683875 nsec\nrounds: 147646"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1542750.8498775144,
            "unit": "iter/sec",
            "range": "stddev: 5.998772823236256e-8",
            "extra": "mean: 648.1928044826393 nsec\nrounds: 152859"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6498417.864146011,
            "unit": "iter/sec",
            "range": "stddev: 8.627994453727145e-9",
            "extra": "mean: 153.88360996560488 nsec\nrounds: 63984"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6473013.907482575,
            "unit": "iter/sec",
            "range": "stddev: 8.33141357289833e-9",
            "extra": "mean: 154.48754077972606 nsec\nrounds: 63943"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10386904.83430188,
            "unit": "iter/sec",
            "range": "stddev: 6.371295211181801e-9",
            "extra": "mean: 96.27507096218993 nsec\nrounds: 102166"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "ba7fb25e8c9c94902e2b8bc6cdfe5fb2ba19546f",
          "message": "Add relative benchmark plot",
          "timestamp": "2024-03-15T17:43:34+01:00",
          "tree_id": "c2b7135118f18b3b808d84ad4c1dc5e488d8ee3f",
          "url": "https://github.com/apytypes/apytypes/commit/ba7fb25e8c9c94902e2b8bc6cdfe5fb2ba19546f"
        },
        "date": 1710521252806,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2278931.799763997,
            "unit": "iter/sec",
            "range": "stddev: 3.374051669803958e-8",
            "extra": "mean: 438.80207389425874 nsec\nrounds: 187618"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1712162.1225038723,
            "unit": "iter/sec",
            "range": "stddev: 5.886503546743085e-8",
            "extra": "mean: 584.0568406791193 nsec\nrounds: 171233"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1686198.5094978882,
            "unit": "iter/sec",
            "range": "stddev: 4.469968199170674e-8",
            "extra": "mean: 593.0499845463194 nsec\nrounds: 164963"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1654942.1186010735,
            "unit": "iter/sec",
            "range": "stddev: 4.3286534823606265e-8",
            "extra": "mean: 604.2507401077345 nsec\nrounds: 162814"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1318192.2701443213,
            "unit": "iter/sec",
            "range": "stddev: 5.163200348625955e-8",
            "extra": "mean: 758.6146745424979 nsec\nrounds: 129803"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6369524.513244064,
            "unit": "iter/sec",
            "range": "stddev: 7.3499733226182945e-9",
            "extra": "mean: 156.99759031001435 nsec\nrounds: 62618"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6464893.610036226,
            "unit": "iter/sec",
            "range": "stddev: 7.180921957938655e-9",
            "extra": "mean: 154.6815864761355 nsec\nrounds: 63537"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10480878.064466443,
            "unit": "iter/sec",
            "range": "stddev: 5.404587656819499e-9",
            "extra": "mean: 95.41185326732122 nsec\nrounds: 102376"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "fd93e3629cd4d50702706a40146f8d7ca4afc002",
          "message": "Special case short negations",
          "timestamp": "2024-03-15T18:35:57+01:00",
          "tree_id": "9f5f101b34999448b5e13a4ac81d084c5952c5ef",
          "url": "https://github.com/apytypes/apytypes/commit/fd93e3629cd4d50702706a40146f8d7ca4afc002"
        },
        "date": 1710524385409,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2252755.6912362254,
            "unit": "iter/sec",
            "range": "stddev: 4.16050858951857e-8",
            "extra": "mean: 443.9007762316435 nsec\nrounds: 191976"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1688606.290658945,
            "unit": "iter/sec",
            "range": "stddev: 5.3047814880546e-8",
            "extra": "mean: 592.2043554686998 nsec\nrounds: 160488"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1651995.1727294058,
            "unit": "iter/sec",
            "range": "stddev: 4.9510901976627364e-8",
            "extra": "mean: 605.3286453300025 nsec\nrounds: 163908"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1333167.0305512897,
            "unit": "iter/sec",
            "range": "stddev: 9.258690124579502e-8",
            "extra": "mean: 750.0935569840581 nsec\nrounds: 130141"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1266109.9720280245,
            "unit": "iter/sec",
            "range": "stddev: 6.044597496274315e-8",
            "extra": "mean: 789.8208071121887 nsec\nrounds: 118695"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6818210.4160097595,
            "unit": "iter/sec",
            "range": "stddev: 1.0215599534691965e-8",
            "extra": "mean: 146.666051498168 nsec\nrounds: 66410"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6773728.345232082,
            "unit": "iter/sec",
            "range": "stddev: 1.1876868507362338e-8",
            "extra": "mean: 147.62918573548595 nsec\nrounds: 64856"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10502276.925334038,
            "unit": "iter/sec",
            "range": "stddev: 6.774645844496197e-9",
            "extra": "mean: 95.21744733160872 nsec\nrounds: 111025"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "committer": {
            "email": "oscar.gustafsson@gmail.com",
            "name": "Oscar Gustafsson",
            "username": "oscargus"
          },
          "distinct": true,
          "id": "dadfb6c0a1245b2d954e945b0bf6cca7715fd61d",
          "message": "Special case short abs",
          "timestamp": "2024-03-15T18:55:40+01:00",
          "tree_id": "ee091c534560f949b05acda375c11f87d6d15196",
          "url": "https://github.com/apytypes/apytypes/commit/dadfb6c0a1245b2d954e945b0bf6cca7715fd61d"
        },
        "date": 1710525573356,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2219861.7101688483,
            "unit": "iter/sec",
            "range": "stddev: 4.062008002762289e-8",
            "extra": "mean: 450.4785119807921 nsec\nrounds: 184502"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1667523.4296865605,
            "unit": "iter/sec",
            "range": "stddev: 5.94636981668217e-8",
            "extra": "mean: 599.6917237846571 nsec\nrounds: 163106"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1395438.1872660818,
            "unit": "iter/sec",
            "range": "stddev: 6.064340229559879e-8",
            "extra": "mean: 716.6207784231378 nsec\nrounds: 137099"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1279300.0605831784,
            "unit": "iter/sec",
            "range": "stddev: 7.11478066393487e-8",
            "extra": "mean: 781.6774428543071 nsec\nrounds: 128784"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1014082.5265846143,
            "unit": "iter/sec",
            "range": "stddev: 7.550014710539712e-8",
            "extra": "mean: 986.1130369418353 nsec\nrounds: 100614"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5846566.635181125,
            "unit": "iter/sec",
            "range": "stddev: 8.20057357101067e-9",
            "extra": "mean: 171.04055463637147 nsec\nrounds: 58579"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5802640.1799095515,
            "unit": "iter/sec",
            "range": "stddev: 9.517655842182744e-9",
            "extra": "mean: 172.33534546262715 nsec\nrounds: 58371"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10221047.091181222,
            "unit": "iter/sec",
            "range": "stddev: 5.833370075956933e-9",
            "extra": "mean: 97.8373341868942 nsec\nrounds: 100716"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "committer": {
            "email": "mikael.henriksson@liu.se",
            "name": "Mikael Henriksson",
            "username": "miklhh"
          },
          "distinct": false,
          "id": "8008856ed8e653956a4df2555f3a53bb03db7cde",
          "message": "src: add APyFixedArray::_set_values_from_numpy_ndarray()",
          "timestamp": "2024-03-16T09:46:15+01:00",
          "tree_id": "d5bddbe20db7813749d1e5930f993d30f862cca9",
          "url": "https://github.com/apytypes/apytypes/commit/8008856ed8e653956a4df2555f3a53bb03db7cde"
        },
        "date": 1710579714900,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2272044.300752643,
            "unit": "iter/sec",
            "range": "stddev: 3.644838296687393e-8",
            "extra": "mean: 440.1322631203695 nsec\nrounds: 190477"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1612888.4541755114,
            "unit": "iter/sec",
            "range": "stddev: 5.7055330921004007e-8",
            "extra": "mean: 620.0056782669302 nsec\nrounds: 158429"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1311440.7912236971,
            "unit": "iter/sec",
            "range": "stddev: 5.4086372649001036e-8",
            "extra": "mean: 762.5201280090165 nsec\nrounds: 127812"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1513540.4151687764,
            "unit": "iter/sec",
            "range": "stddev: 5.227957633062721e-8",
            "extra": "mean: 660.7025421837908 nsec\nrounds: 151012"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1286646.5917124252,
            "unit": "iter/sec",
            "range": "stddev: 6.989586179748952e-8",
            "extra": "mean: 777.214198864858 nsec\nrounds: 128123"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6658845.997302294,
            "unit": "iter/sec",
            "range": "stddev: 9.522313071154445e-9",
            "extra": "mean: 150.17617172780703 nsec\nrounds: 66850"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6626913.6282195,
            "unit": "iter/sec",
            "range": "stddev: 1.0412109935985405e-8",
            "extra": "mean: 150.89980888563434 nsec\nrounds: 66191"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10521224.31454599,
            "unit": "iter/sec",
            "range": "stddev: 6.1417886893663585e-9",
            "extra": "mean: 95.04597279780909 nsec\nrounds: 104189"
          }
        ]
      }
    ]
  }
}