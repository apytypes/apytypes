window.BENCHMARK_DATA = {
  "lastUpdate": 1711030000371,
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
          "id": "1085f94a22bc3a38032b3bf5229073b8456b1983",
          "message": "More fixed-point array benchmarks",
          "timestamp": "2024-03-16T15:46:49+01:00",
          "tree_id": "eab1295361478d03a914832f57e36855cbc57364",
          "url": "https://github.com/apytypes/apytypes/commit/1085f94a22bc3a38032b3bf5229073b8456b1983"
        },
        "date": 1710600605740,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 23671.55129226702,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015152044408039185",
            "extra": "mean: 42.244802110906775 usec\nrounds: 23311"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 109753.4703744327,
            "unit": "iter/sec",
            "range": "stddev: 8.073963269031506e-7",
            "extra": "mean: 9.111329205249003 usec\nrounds: 104406"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 918.9660183008673,
            "unit": "iter/sec",
            "range": "stddev: 0.000126035299197311",
            "extra": "mean: 1.0881795192481236 msec\nrounds: 1065"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 535.9314977015634,
            "unit": "iter/sec",
            "range": "stddev: 0.00004648271260101649",
            "extra": "mean: 1.8659101103194642 msec\nrounds: 562"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 489.51095389283466,
            "unit": "iter/sec",
            "range": "stddev: 0.00005265284827292905",
            "extra": "mean: 2.0428552048682516 msec\nrounds: 493"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 101703.5158234797,
            "unit": "iter/sec",
            "range": "stddev: 6.32675343350923e-7",
            "extra": "mean: 9.832501776395185 usec\nrounds: 95694"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 1069.0147312791269,
            "unit": "iter/sec",
            "range": "stddev: 0.0000865326867181448",
            "extra": "mean: 935.4408042660484 usec\nrounds: 1078"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 952208.3439864313,
            "unit": "iter/sec",
            "range": "stddev: 6.22563354772876e-8",
            "extra": "mean: 1.0501903352510982 usec\nrounds: 96526"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17349.2222859673,
            "unit": "iter/sec",
            "range": "stddev: 0.000002507703128884663",
            "extra": "mean: 57.639471298309296 usec\nrounds: 16323"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 179401.7502115366,
            "unit": "iter/sec",
            "range": "stddev: 5.274021984461794e-7",
            "extra": "mean: 5.574081628640064 usec\nrounds: 166890"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 1902.8283620795605,
            "unit": "iter/sec",
            "range": "stddev: 0.00010683627728291063",
            "extra": "mean: 525.5334742368046 usec\nrounds: 1999"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 191605.4670511406,
            "unit": "iter/sec",
            "range": "stddev: 3.711630910780095e-7",
            "extra": "mean: 5.219057761713523 usec\nrounds: 132891"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2121.993684146128,
            "unit": "iter/sec",
            "range": "stddev: 0.000024700513922448333",
            "extra": "mean: 471.2549370298392 usec\nrounds: 2128"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 186141.0616510439,
            "unit": "iter/sec",
            "range": "stddev: 0.000001530440936568967",
            "extra": "mean: 5.372269778253903 usec\nrounds: 165536"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2207.1578984509188,
            "unit": "iter/sec",
            "range": "stddev: 0.000016782309201526622",
            "extra": "mean: 453.071346051791 usec\nrounds: 2254"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 211558.76079839154,
            "unit": "iter/sec",
            "range": "stddev: 4.3853233096742505e-7",
            "extra": "mean: 4.726819141056355 usec\nrounds: 190115"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2482.1117750286344,
            "unit": "iter/sec",
            "range": "stddev: 0.000004234421224324314",
            "extra": "mean: 402.8827428565193 usec\nrounds: 2485"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 163734.23029538515,
            "unit": "iter/sec",
            "range": "stddev: 4.6944350869393637e-7",
            "extra": "mean: 6.107458398869603 usec\nrounds: 143205"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2027.9925146595097,
            "unit": "iter/sec",
            "range": "stddev: 0.00002415990687391946",
            "extra": "mean: 493.0984669674164 usec\nrounds: 1998"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 61307.358216954424,
            "unit": "iter/sec",
            "range": "stddev: 8.761008201910372e-7",
            "extra": "mean: 16.31125576250082 usec\nrounds: 53405"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 681.0232098106751,
            "unit": "iter/sec",
            "range": "stddev: 0.000019348417029511965",
            "extra": "mean: 1.4683787359875746 msec\nrounds: 678"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 134784.53161299447,
            "unit": "iter/sec",
            "range": "stddev: 5.429624461867229e-7",
            "extra": "mean: 7.419248989723022 usec\nrounds: 104189"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 159367.2806132077,
            "unit": "iter/sec",
            "range": "stddev: 4.6399259813721956e-7",
            "extra": "mean: 6.274813726834241 usec\nrounds: 133068"
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
          "id": "3cdd779f89b4253f80f05a35d5f25be8fb8e9dca",
          "message": "Compute shift amount less often",
          "timestamp": "2024-03-16T18:19:28+01:00",
          "tree_id": "181c28f8026c39ffd5cf445e4725d749df290e55",
          "url": "https://github.com/apytypes/apytypes/commit/3cdd779f89b4253f80f05a35d5f25be8fb8e9dca"
        },
        "date": 1710609762325,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 23298.207573966178,
            "unit": "iter/sec",
            "range": "stddev: 0.0000037581941835913205",
            "extra": "mean: 42.921756827225515 usec\nrounds: 18419"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 117100.28459487495,
            "unit": "iter/sec",
            "range": "stddev: 7.483041203523355e-7",
            "extra": "mean: 8.539688895374098 usec\nrounds: 113547"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 976.5224453798303,
            "unit": "iter/sec",
            "range": "stddev: 0.0000397553406911952",
            "extra": "mean: 1.0240420020361516 msec\nrounds: 981"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 530.117215535911,
            "unit": "iter/sec",
            "range": "stddev: 0.000057055691710388086",
            "extra": "mean: 1.8863752594585537 msec\nrounds: 555"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 475.6120591891429,
            "unit": "iter/sec",
            "range": "stddev: 0.00007467857216061995",
            "extra": "mean: 2.1025539211618622 msec\nrounds: 482"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 106901.48966638371,
            "unit": "iter/sec",
            "range": "stddev: 0.0000010939032241146314",
            "extra": "mean: 9.35440659546263 usec\nrounds: 104406"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 1149.2925141895732,
            "unit": "iter/sec",
            "range": "stddev: 0.000031505648390649905",
            "extra": "mean: 870.100507619814 usec\nrounds: 1050"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 914603.2618163209,
            "unit": "iter/sec",
            "range": "stddev: 8.641005414386024e-8",
            "extra": "mean: 1.0933702532550553 usec\nrounds: 75104"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 16496.84942055965,
            "unit": "iter/sec",
            "range": "stddev: 0.0000029953775742246153",
            "extra": "mean: 60.61763519243393 usec\nrounds: 13662"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 184670.4225902893,
            "unit": "iter/sec",
            "range": "stddev: 4.162670245203708e-7",
            "extra": "mean: 5.415052318467938 usec\nrounds: 174795"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 2105.1412283678806,
            "unit": "iter/sec",
            "range": "stddev: 0.000018593001630215256",
            "extra": "mean: 475.02751194289306 usec\nrounds: 1842"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 196241.91711781375,
            "unit": "iter/sec",
            "range": "stddev: 4.558798288846113e-7",
            "extra": "mean: 5.095751278253415 usec\nrounds: 133977"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 2215.6799437145405,
            "unit": "iter/sec",
            "range": "stddev: 0.000016544934641066523",
            "extra": "mean: 451.3287231925389 usec\nrounds: 2117"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 194357.34297347453,
            "unit": "iter/sec",
            "range": "stddev: 4.6700008943160145e-7",
            "extra": "mean: 5.145161920311279 usec\nrounds: 171498"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 2288.7538199358723,
            "unit": "iter/sec",
            "range": "stddev: 0.00002294410527192382",
            "extra": "mean: 436.9189867820815 usec\nrounds: 2194"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 194716.91958893603,
            "unit": "iter/sec",
            "range": "stddev: 4.644495103418875e-7",
            "extra": "mean: 5.135660537929036 usec\nrounds: 157184"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2294.9073058366803,
            "unit": "iter/sec",
            "range": "stddev: 0.00003191417166241694",
            "extra": "mean: 435.74744716559206 usec\nrounds: 2082"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 156222.77218652444,
            "unit": "iter/sec",
            "range": "stddev: 5.369876143581642e-7",
            "extra": "mean: 6.401115445615289 usec\nrounds: 127324"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 1941.7832417763616,
            "unit": "iter/sec",
            "range": "stddev: 0.00003464482746963807",
            "extra": "mean: 514.9905398736424 usec\nrounds: 1906"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 53142.4208859869,
            "unit": "iter/sec",
            "range": "stddev: 0.000001642390555628546",
            "extra": "mean: 18.81735877530731 usec\nrounds: 48175"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 588.3309691168163,
            "unit": "iter/sec",
            "range": "stddev: 0.00008491621447197242",
            "extra": "mean: 1.6997235442172427 msec\nrounds: 588"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 128580.90885102758,
            "unit": "iter/sec",
            "range": "stddev: 7.389037961950082e-7",
            "extra": "mean: 7.77720432166636 usec\nrounds: 102691"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 148674.49100487508,
            "unit": "iter/sec",
            "range": "stddev: 7.587944642232038e-7",
            "extra": "mean: 6.7261034037588185 usec\nrounds: 125866"
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
          "id": "a78d22c8c9593f9c803cb11afde930b4e65a5d70",
          "message": "Inline scalar cast in array",
          "timestamp": "2024-03-16T19:17:31+01:00",
          "tree_id": "eeb97c983d61e9e692a2ea000ecf242953c9313e",
          "url": "https://github.com/apytypes/apytypes/commit/a78d22c8c9593f9c803cb11afde930b4e65a5d70"
        },
        "date": 1710613285521,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25348.135120009687,
            "unit": "iter/sec",
            "range": "stddev: 0.000001636018918027251",
            "extra": "mean: 39.45063395257843 usec\nrounds: 24658"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 190568.56331906668,
            "unit": "iter/sec",
            "range": "stddev: 3.777961591471477e-7",
            "extra": "mean: 5.247455207634178 usec\nrounds: 170911"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1466.411946865786,
            "unit": "iter/sec",
            "range": "stddev: 0.00001411319167649603",
            "extra": "mean: 681.936615517444 usec\nrounds: 1753"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 636.5028735861956,
            "unit": "iter/sec",
            "range": "stddev: 0.00001422226457777528",
            "extra": "mean: 1.5710848159502917 msec\nrounds: 652"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 565.4231259649539,
            "unit": "iter/sec",
            "range": "stddev: 0.000023371015061396856",
            "extra": "mean: 1.76858701754265 msec\nrounds: 570"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 164297.01352673382,
            "unit": "iter/sec",
            "range": "stddev: 5.063706362495813e-7",
            "extra": "mean: 6.086537901903394 usec\nrounds: 151470"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 1827.6228529906166,
            "unit": "iter/sec",
            "range": "stddev: 0.000008588376404569268",
            "extra": "mean: 547.1588398906579 usec\nrounds: 1830"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 999698.1935664946,
            "unit": "iter/sec",
            "range": "stddev: 6.558246273485362e-8",
            "extra": "mean: 1.0003018975481495 usec\nrounds: 101236"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17030.30252984161,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021506526156595093",
            "extra": "mean: 58.71886293550773 usec\nrounds: 17189"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 282993.9594483138,
            "unit": "iter/sec",
            "range": "stddev: 2.5468048179542577e-7",
            "extra": "mean: 3.5336443291915587 usec\nrounds: 137855"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 3271.766436179881,
            "unit": "iter/sec",
            "range": "stddev: 0.000008101730337761768",
            "extra": "mean: 305.64528963369446 usec\nrounds: 3280"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 285931.365622784,
            "unit": "iter/sec",
            "range": "stddev: 4.196327888014049e-7",
            "extra": "mean: 3.4973427900150473 usec\nrounds: 170040"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 3519.7270050620855,
            "unit": "iter/sec",
            "range": "stddev: 0.000006967601671135384",
            "extra": "mean: 284.1129435782366 usec\nrounds: 3527"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 301398.062948121,
            "unit": "iter/sec",
            "range": "stddev: 2.658602701666674e-7",
            "extra": "mean: 3.317871356632202 usec\nrounds: 148523"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 3744.911511740586,
            "unit": "iter/sec",
            "range": "stddev: 0.000010107089412808083",
            "extra": "mean: 267.0290063903841 usec\nrounds: 3756"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 211453.1156029853,
            "unit": "iter/sec",
            "range": "stddev: 4.191683890018789e-7",
            "extra": "mean: 4.729180731853365 usec\nrounds: 185529"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2483.687638615475,
            "unit": "iter/sec",
            "range": "stddev: 0.000005373892084662433",
            "extra": "mean: 402.62711963145546 usec\nrounds: 2491"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 167410.7610431484,
            "unit": "iter/sec",
            "range": "stddev: 4.843511694019835e-7",
            "extra": "mean: 5.9733316649953 usec\nrounds: 147450"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2088.186376130432,
            "unit": "iter/sec",
            "range": "stddev: 0.000005487179199069795",
            "extra": "mean: 478.8844575516654 usec\nrounds: 2026"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 56145.24836071187,
            "unit": "iter/sec",
            "range": "stddev: 0.0000011459338968454624",
            "extra": "mean: 17.810946236721232 usec\nrounds: 50462"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 618.5928881095906,
            "unit": "iter/sec",
            "range": "stddev: 0.000025945438886278638",
            "extra": "mean: 1.616572093248571 msec\nrounds: 622"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 141217.3325605713,
            "unit": "iter/sec",
            "range": "stddev: 5.833325508025121e-7",
            "extra": "mean: 7.081283733858077 usec\nrounds: 104954"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 157100.25677463587,
            "unit": "iter/sec",
            "range": "stddev: 5.406143544288041e-7",
            "extra": "mean: 6.365361970315073 usec\nrounds: 134337"
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
          "id": "0e8326562c018e28bef8d0facf1f1b0b82128c20",
          "message": "Remove unused arguments",
          "timestamp": "2024-03-16T19:30:41+01:00",
          "tree_id": "3eb7bbd376c47582af0b56edcd3bd0a27378db8e",
          "url": "https://github.com/apytypes/apytypes/commit/0e8326562c018e28bef8d0facf1f1b0b82128c20"
        },
        "date": 1710614018580,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24981.862925884827,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015403956041327477",
            "extra": "mean: 40.02904038689026 usec\nrounds: 24092"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 190502.80060448986,
            "unit": "iter/sec",
            "range": "stddev: 3.9121397146439314e-7",
            "extra": "mean: 5.249266660788564 usec\nrounds: 170040"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 1457.927910015525,
            "unit": "iter/sec",
            "range": "stddev: 0.000025460907242049813",
            "extra": "mean: 685.9049704243273 usec\nrounds: 1792"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 638.0851113186199,
            "unit": "iter/sec",
            "range": "stddev: 0.00004869273665963301",
            "extra": "mean: 1.5671890508986699 msec\nrounds: 668"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 566.5743138320474,
            "unit": "iter/sec",
            "range": "stddev: 0.000015492568745861632",
            "extra": "mean: 1.7649935332162539 msec\nrounds: 572"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 166759.93795090303,
            "unit": "iter/sec",
            "range": "stddev: 3.8901244742482784e-7",
            "extra": "mean: 5.996644111815495 usec\nrounds: 155473"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 1870.974027065329,
            "unit": "iter/sec",
            "range": "stddev: 0.000006150280144161255",
            "extra": "mean: 534.480963142244 usec\nrounds: 1872"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1014268.8306677652,
            "unit": "iter/sec",
            "range": "stddev: 4.264028031900891e-8",
            "extra": "mean: 985.9319046032758 nsec\nrounds: 50488"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17250.216227921213,
            "unit": "iter/sec",
            "range": "stddev: 0.0000023390620607146516",
            "extra": "mean: 57.97028783798079 usec\nrounds: 17555"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 284163.2423117828,
            "unit": "iter/sec",
            "range": "stddev: 2.3153092120009958e-7",
            "extra": "mean: 3.5191039905956734 usec\nrounds: 141383"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 3280.2909389794845,
            "unit": "iter/sec",
            "range": "stddev: 0.000005120312744847809",
            "extra": "mean: 304.8510082191384 usec\nrounds: 3285"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 287933.8027459392,
            "unit": "iter/sec",
            "range": "stddev: 3.3281699578750373e-7",
            "extra": "mean: 3.4730205014600477 usec\nrounds: 170329"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 3949.2466836950593,
            "unit": "iter/sec",
            "range": "stddev: 0.00000904398819553959",
            "extra": "mean: 253.2128479410062 usec\nrounds: 3959"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 301131.10959549295,
            "unit": "iter/sec",
            "range": "stddev: 2.0582382801607053e-7",
            "extra": "mean: 3.3208126564647937 usec\nrounds: 149410"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 3757.343690359214,
            "unit": "iter/sec",
            "range": "stddev: 0.000008701894674856615",
            "extra": "mean: 266.1454693553457 usec\nrounds: 3769"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 211325.76561563637,
            "unit": "iter/sec",
            "range": "stddev: 3.576856418244526e-7",
            "extra": "mean: 4.732030649867942 usec\nrounds: 190115"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2481.601373510679,
            "unit": "iter/sec",
            "range": "stddev: 0.00001421239489903942",
            "extra": "mean: 402.9656054651989 usec\nrounds: 2489"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 168222.06062009925,
            "unit": "iter/sec",
            "range": "stddev: 3.8860810524708564e-7",
            "extra": "mean: 5.944523544140439 usec\nrounds: 143411"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2087.2894812027484,
            "unit": "iter/sec",
            "range": "stddev: 0.000018026284163512064",
            "extra": "mean: 479.09023113735765 usec\nrounds: 2094"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 56238.91350771019,
            "unit": "iter/sec",
            "range": "stddev: 8.33838272657418e-7",
            "extra": "mean: 17.78128234754932 usec\nrounds: 50746"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 618.9430624957795,
            "unit": "iter/sec",
            "range": "stddev: 0.000045992621908254586",
            "extra": "mean: 1.615657498393592 msec\nrounds: 622"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 139616.9064426144,
            "unit": "iter/sec",
            "range": "stddev: 5.544661061341871e-7",
            "extra": "mean: 7.162456363485048 usec\nrounds: 105955"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 166150.1110709524,
            "unit": "iter/sec",
            "range": "stddev: 4.499039456842688e-7",
            "extra": "mean: 6.018653815843445 usec\nrounds: 132909"
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
          "id": "7bbc14782cc3e9012e2c2a2fc782850da70caed2",
          "message": "Add test",
          "timestamp": "2024-03-16T20:08:08+01:00",
          "tree_id": "9e2df6f83d929296daf4e20e9b411e0019de64cd",
          "url": "https://github.com/apytypes/apytypes/commit/7bbc14782cc3e9012e2c2a2fc782850da70caed2"
        },
        "date": 1710616269652,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24625.219831535527,
            "unit": "iter/sec",
            "range": "stddev: 0.0000026243565108111626",
            "extra": "mean: 40.60877453444622 usec\nrounds: 24221"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 697981.7724797074,
            "unit": "iter/sec",
            "range": "stddev: 1.2320888740755246e-7",
            "extra": "mean: 1.432702169925328 usec\nrounds: 70141"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 3041.8983242853383,
            "unit": "iter/sec",
            "range": "stddev: 0.00001216310757548194",
            "extra": "mean: 328.74208582725703 usec\nrounds: 5045"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 728.7289466523026,
            "unit": "iter/sec",
            "range": "stddev: 0.000021765118209056397",
            "extra": "mean: 1.3722523368858688 msec\nrounds: 751"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 623.7924355018621,
            "unit": "iter/sec",
            "range": "stddev: 0.00001410604956827229",
            "extra": "mean: 1.6030973495141316 msec\nrounds: 618"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 322683.7284928089,
            "unit": "iter/sec",
            "range": "stddev: 5.386551981112507e-7",
            "extra": "mean: 3.099009685647305 usec\nrounds: 161787"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 4174.130661563614,
            "unit": "iter/sec",
            "range": "stddev: 0.000006785624702139591",
            "extra": "mean: 239.57084266868725 usec\nrounds: 3928"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1024584.3538141338,
            "unit": "iter/sec",
            "range": "stddev: 6.65196330500046e-8",
            "extra": "mean: 976.005534612551 nsec\nrounds: 100513"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17314.58695683329,
            "unit": "iter/sec",
            "range": "stddev: 0.0000030445749580415593",
            "extra": "mean: 57.75477073135406 usec\nrounds: 16967"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 588530.0085292293,
            "unit": "iter/sec",
            "range": "stddev: 8.075133088227464e-8",
            "extra": "mean: 1.6991487018632727 usec\nrounds: 58507"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 7323.525415239558,
            "unit": "iter/sec",
            "range": "stddev: 0.000003831864945272808",
            "extra": "mean: 136.54625925365062 usec\nrounds: 7294"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 659217.9561690125,
            "unit": "iter/sec",
            "range": "stddev: 1.1858655909843544e-7",
            "extra": "mean: 1.516948970582374 usec\nrounds: 160721"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 13501.925427732138,
            "unit": "iter/sec",
            "range": "stddev: 0.0000024326607177994396",
            "extra": "mean: 74.06351081943176 usec\nrounds: 13263"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 715642.1921961551,
            "unit": "iter/sec",
            "range": "stddev: 7.314950684772164e-8",
            "extra": "mean: 1.3973463427738744 usec\nrounds: 72596"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 11470.729502382308,
            "unit": "iter/sec",
            "range": "stddev: 0.0000024349463425576098",
            "extra": "mean: 87.17841352568851 usec\nrounds: 11223"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 209803.49897155171,
            "unit": "iter/sec",
            "range": "stddev: 3.788995335764395e-7",
            "extra": "mean: 4.766364740826344 usec\nrounds: 185529"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2482.972566173184,
            "unit": "iter/sec",
            "range": "stddev: 0.000006069116479829696",
            "extra": "mean: 402.74307240583965 usec\nrounds: 2486"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 164406.3037922348,
            "unit": "iter/sec",
            "range": "stddev: 4.955404602932682e-7",
            "extra": "mean: 6.082491832331017 usec\nrounds: 144655"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2089.0890245536743,
            "unit": "iter/sec",
            "range": "stddev: 0.000006290384343335034",
            "extra": "mean: 478.67754233864974 usec\nrounds: 1984"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 55890.464829992714,
            "unit": "iter/sec",
            "range": "stddev: 0.0000011621367149135433",
            "extra": "mean: 17.892139617049065 usec\nrounds: 50746"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 618.5367559242012,
            "unit": "iter/sec",
            "range": "stddev: 0.000013277056733121703",
            "extra": "mean: 1.616718797100144 msec\nrounds: 621"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 136852.6314246592,
            "unit": "iter/sec",
            "range": "stddev: 5.497431239525232e-7",
            "extra": "mean: 7.307130228990336 usec\nrounds: 102481"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 154283.4244547073,
            "unit": "iter/sec",
            "range": "stddev: 5.177598354000577e-7",
            "extra": "mean: 6.481577677798875 usec\nrounds: 79096"
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
          "id": "cb733a73063b35d56a76fb9fcd535011749fcd08",
          "message": "Add early return for cast",
          "timestamp": "2024-03-16T21:27:41+01:00",
          "tree_id": "ae4553afcd999df438dddc2d3d0ff31514975ddb",
          "url": "https://github.com/apytypes/apytypes/commit/cb733a73063b35d56a76fb9fcd535011749fcd08"
        },
        "date": 1710621101842,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25339.94292941663,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018252374651413914",
            "extra": "mean: 39.46338801099351 usec\nrounds: 24489"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 662854.8713126852,
            "unit": "iter/sec",
            "range": "stddev: 1.9622531926619637e-7",
            "extra": "mean: 1.508625859563571 usec\nrounds: 197629"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 3029.154589635712,
            "unit": "iter/sec",
            "range": "stddev: 0.000009002562277868347",
            "extra": "mean: 330.12511260452396 usec\nrounds: 4316"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 754.9213569111913,
            "unit": "iter/sec",
            "range": "stddev: 0.0000400953789302239",
            "extra": "mean: 1.3246412899107314 msec\nrounds: 783"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 619.8685016186053,
            "unit": "iter/sec",
            "range": "stddev: 0.000015502544903726414",
            "extra": "mean: 1.6132453857371243 msec\nrounds: 617"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 331670.211476077,
            "unit": "iter/sec",
            "range": "stddev: 2.577436521822371e-7",
            "extra": "mean: 3.0150431525024946 usec\nrounds: 164718"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 3946.9062533479823,
            "unit": "iter/sec",
            "range": "stddev: 0.000010752923426220365",
            "extra": "mean: 253.3629977027565 usec\nrounds: 3918"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1051049.6418928648,
            "unit": "iter/sec",
            "range": "stddev: 6.137493139432955e-8",
            "extra": "mean: 951.4298470232601 nsec\nrounds: 105508"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17234.105749148912,
            "unit": "iter/sec",
            "range": "stddev: 0.000002886230613011626",
            "extra": "mean: 58.0244785865599 usec\nrounds: 17092"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 628620.2353924037,
            "unit": "iter/sec",
            "range": "stddev: 7.76941858457813e-8",
            "extra": "mean: 1.590785570839563 usec\nrounds: 63739"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 7872.17963974282,
            "unit": "iter/sec",
            "range": "stddev: 0.000005509922864947674",
            "extra": "mean: 127.02962149789936 usec\nrounds: 7852"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 718326.5840554535,
            "unit": "iter/sec",
            "range": "stddev: 1.0349206932172714e-7",
            "extra": "mean: 1.3921244489578877 usec\nrounds: 171498"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 9484.563123216305,
            "unit": "iter/sec",
            "range": "stddev: 0.0000050701843835488045",
            "extra": "mean: 105.43448201132226 usec\nrounds: 9645"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 725419.5773570869,
            "unit": "iter/sec",
            "range": "stddev: 7.842798458107912e-8",
            "extra": "mean: 1.378512561851861 usec\nrounds: 71757"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 11377.643124087026,
            "unit": "iter/sec",
            "range": "stddev: 0.000004053718295226329",
            "extra": "mean: 87.8916651800188 usec\nrounds: 11209"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 211921.88143355006,
            "unit": "iter/sec",
            "range": "stddev: 4.275117950576206e-7",
            "extra": "mean: 4.718719903935727 usec\nrounds: 196079"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2482.9615312245132,
            "unit": "iter/sec",
            "range": "stddev: 0.000013456102563203583",
            "extra": "mean: 402.74486230434417 usec\nrounds: 2491"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 164389.2203466681,
            "unit": "iter/sec",
            "range": "stddev: 4.7388098329565587e-7",
            "extra": "mean: 6.083123929240465 usec\nrounds: 134157"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2035.3787805308502,
            "unit": "iter/sec",
            "range": "stddev: 0.000018181086478241358",
            "extra": "mean: 491.30904260443776 usec\nrounds: 2042"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 62412.050878576214,
            "unit": "iter/sec",
            "range": "stddev: 8.420021707906998e-7",
            "extra": "mean: 16.022546702487286 usec\nrounds: 54783"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 700.5850934923195,
            "unit": "iter/sec",
            "range": "stddev: 0.0000504472978092919",
            "extra": "mean: 1.4273783574456869 msec\nrounds: 705"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 143142.70000057408,
            "unit": "iter/sec",
            "range": "stddev: 5.106751105728148e-7",
            "extra": "mean: 6.986035613384331 usec\nrounds: 109566"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 162514.6399004867,
            "unit": "iter/sec",
            "range": "stddev: 4.777567310665672e-7",
            "extra": "mean: 6.153291793356797 usec\nrounds: 131857"
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
          "id": "6f11740433df726d742b3675ec23303d2ca5a661",
          "message": "Add early return for cast and no quantization casting for arrays",
          "timestamp": "2024-03-16T23:56:33+01:00",
          "tree_id": "4373ecb5b36dff857ebbb054130efae2a3c81e8a",
          "url": "https://github.com/apytypes/apytypes/commit/6f11740433df726d742b3675ec23303d2ca5a661"
        },
        "date": 1710629982299,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24708.149904941958,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016218402728129607",
            "extra": "mean: 40.47247583680827 usec\nrounds: 24128"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 630869.9145204879,
            "unit": "iter/sec",
            "range": "stddev: 1.3551867500506128e-7",
            "extra": "mean: 1.5851128370261065 usec\nrounds: 60902"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 4138.133262654489,
            "unit": "iter/sec",
            "range": "stddev: 0.0001132426306451142",
            "extra": "mean: 241.6548565568741 usec\nrounds: 7313"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 753.7008198716205,
            "unit": "iter/sec",
            "range": "stddev: 0.000021289734398041924",
            "extra": "mean: 1.326786403350778 msec\nrounds: 776"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 615.7001904016665,
            "unit": "iter/sec",
            "range": "stddev: 0.000039804250416647515",
            "extra": "mean: 1.6241671118334828 msec\nrounds: 617"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 311168.87986472284,
            "unit": "iter/sec",
            "range": "stddev: 5.597080576388918e-7",
            "extra": "mean: 3.2136889795494294 usec\nrounds: 158705"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 3771.0646307474126,
            "unit": "iter/sec",
            "range": "stddev: 0.000010126382565844691",
            "extra": "mean: 265.17710458910994 usec\nrounds: 3595"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1045990.5970149199,
            "unit": "iter/sec",
            "range": "stddev: 6.504842120470806e-8",
            "extra": "mean: 956.0315387670064 nsec\nrounds: 101751"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17209.247711787604,
            "unit": "iter/sec",
            "range": "stddev: 0.0000029976946324586418",
            "extra": "mean: 58.108292515020416 usec\nrounds: 17329"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 632137.7734335379,
            "unit": "iter/sec",
            "range": "stddev: 8.423237420890926e-8",
            "extra": "mean: 1.5819336259062184 usec\nrounds: 62779"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 7878.976604001875,
            "unit": "iter/sec",
            "range": "stddev: 0.000005497092757475538",
            "extra": "mean: 126.92003673320744 usec\nrounds: 7786"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 708704.7133108522,
            "unit": "iter/sec",
            "range": "stddev: 1.0465380932026345e-7",
            "extra": "mean: 1.4110249039099867 usec\nrounds: 181819"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 9536.1394375221,
            "unit": "iter/sec",
            "range": "stddev: 0.00000295012282055256",
            "extra": "mean: 104.86423846376171 usec\nrounds: 9557"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 635316.2339246909,
            "unit": "iter/sec",
            "range": "stddev: 2.713250417760462e-7",
            "extra": "mean: 1.574019278277279 usec\nrounds: 74488"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 7447.687754194694,
            "unit": "iter/sec",
            "range": "stddev: 0.000022718867355769295",
            "extra": "mean: 134.26986106349304 usec\nrounds: 11185"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 211744.36064078676,
            "unit": "iter/sec",
            "range": "stddev: 4.186515642002033e-7",
            "extra": "mean: 4.722675952142347 usec\nrounds: 192308"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2482.33534309817,
            "unit": "iter/sec",
            "range": "stddev: 0.000006575586151383066",
            "extra": "mean: 402.8464577843513 usec\nrounds: 2357"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 161261.45252854118,
            "unit": "iter/sec",
            "range": "stddev: 5.390010342093549e-7",
            "extra": "mean: 6.201109963480039 usec\nrounds: 140965"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2003.598532054674,
            "unit": "iter/sec",
            "range": "stddev: 0.00002862622030633168",
            "extra": "mean: 499.1019827582466 usec\nrounds: 1972"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 62037.15783198084,
            "unit": "iter/sec",
            "range": "stddev: 0.0000011203579724067525",
            "extra": "mean: 16.119371598363085 usec\nrounds: 54131"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 702.7161048638418,
            "unit": "iter/sec",
            "range": "stddev: 0.000010070138685906235",
            "extra": "mean: 1.4230497822356867 msec\nrounds: 698"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 126550.776691196,
            "unit": "iter/sec",
            "range": "stddev: 6.045804431030477e-7",
            "extra": "mean: 7.901966516097794 usec\nrounds: 104080"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 157842.81699178135,
            "unit": "iter/sec",
            "range": "stddev: 4.336073629505805e-7",
            "extra": "mean: 6.33541658124404 usec\nrounds: 128784"
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
          "id": "794f95feac30dbced9c758d57e8c8bf546703adf",
          "message": "src: add faster APyFixedArray add/sub",
          "timestamp": "2024-03-18T09:40:47+01:00",
          "tree_id": "14f6e7a9f1597c5186a5f0637ca03ad956fabcdd",
          "url": "https://github.com/apytypes/apytypes/commit/794f95feac30dbced9c758d57e8c8bf546703adf"
        },
        "date": 1710751736394,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 23862.659081666774,
            "unit": "iter/sec",
            "range": "stddev: 0.0000017002417007874295",
            "extra": "mean: 41.90647809104732 usec\nrounds: 23073"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1796702.5396017344,
            "unit": "iter/sec",
            "range": "stddev: 4.969913561319704e-8",
            "extra": "mean: 556.5751580792198 nsec\nrounds: 182816"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 47152.949046720314,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018757807581115663",
            "extra": "mean: 21.207581290603375 usec\nrounds: 26682"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 715.5590713635878,
            "unit": "iter/sec",
            "range": "stddev: 0.00003042912904069104",
            "extra": "mean: 1.3975086614363985 msec\nrounds: 765"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 621.3126001579595,
            "unit": "iter/sec",
            "range": "stddev: 0.00001948601351634789",
            "extra": "mean: 1.6094957670997896 msec\nrounds: 614"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1874245.3439289627,
            "unit": "iter/sec",
            "range": "stddev: 4.040340524124667e-8",
            "extra": "mean: 533.5480774910837 nsec\nrounds: 182782"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 46747.091518064255,
            "unit": "iter/sec",
            "range": "stddev: 0.0000013518985430085974",
            "extra": "mean: 21.39170518477229 usec\nrounds: 31708"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 936916.3882920273,
            "unit": "iter/sec",
            "range": "stddev: 6.112380194391495e-8",
            "extra": "mean: 1.067331100721772 usec\nrounds: 93284"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 15382.899316379151,
            "unit": "iter/sec",
            "range": "stddev: 0.0000026505905216531144",
            "extra": "mean: 65.00725119712877 usec\nrounds: 14829"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 584774.6246001142,
            "unit": "iter/sec",
            "range": "stddev: 8.304852502474772e-8",
            "extra": "mean: 1.7100605223488112 usec\nrounds: 58542"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 7241.689131878414,
            "unit": "iter/sec",
            "range": "stddev: 0.000005302304402932413",
            "extra": "mean: 138.08932996004086 usec\nrounds: 7016"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 657306.1960242606,
            "unit": "iter/sec",
            "range": "stddev: 1.3237006312016182e-7",
            "extra": "mean: 1.5213609822157996 usec\nrounds: 162285"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 13249.076451054785,
            "unit": "iter/sec",
            "range": "stddev: 0.000002744068430638608",
            "extra": "mean: 75.47695899365031 usec\nrounds: 12876"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1586644.1066901165,
            "unit": "iter/sec",
            "range": "stddev: 6.571227826212116e-8",
            "extra": "mean: 630.2610621900201 nsec\nrounds: 170911"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 53287.541179471766,
            "unit": "iter/sec",
            "range": "stddev: 0.0000013479206475984255",
            "extra": "mean: 18.766112638449812 usec\nrounds: 43644"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 204993.59222997908,
            "unit": "iter/sec",
            "range": "stddev: 0.000002932900512318545",
            "extra": "mean: 4.878201260447769 usec\nrounds: 192308"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2430.498675135706,
            "unit": "iter/sec",
            "range": "stddev: 0.00007431306513482873",
            "extra": "mean: 411.4381999999096 usec\nrounds: 2460"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 164153.17179637033,
            "unit": "iter/sec",
            "range": "stddev: 5.102358164027451e-7",
            "extra": "mean: 6.091871323939363 usec\nrounds: 141184"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 1952.1669065096676,
            "unit": "iter/sec",
            "range": "stddev: 0.00009617548667417757",
            "extra": "mean: 512.2512817246386 usec\nrounds: 2041"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 62579.894961013975,
            "unit": "iter/sec",
            "range": "stddev: 0.000002770599142399006",
            "extra": "mean: 15.979573002207498 usec\nrounds: 53923"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 698.5202516271742,
            "unit": "iter/sec",
            "range": "stddev: 0.00007590146753125415",
            "extra": "mean: 1.4315977205679309 msec\nrounds: 705"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 131864.26272684592,
            "unit": "iter/sec",
            "range": "stddev: 0.0000011104358507883884",
            "extra": "mean: 7.583555842354947 usec\nrounds: 102691"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 159026.51039776663,
            "unit": "iter/sec",
            "range": "stddev: 4.639181882654465e-7",
            "extra": "mean: 6.28825972159447 usec\nrounds: 125392"
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
          "id": "d18aef6266a3990a9fcfd7bafd748d9aab5b9a81",
          "message": "Faster unary operations (and some binary)",
          "timestamp": "2024-03-18T11:03:44+01:00",
          "tree_id": "43144e49a14d27b7d4f4b682e571c77f3d7f4f91",
          "url": "https://github.com/apytypes/apytypes/commit/d18aef6266a3990a9fcfd7bafd748d9aab5b9a81"
        },
        "date": 1710756432120,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 23365.98770526198,
            "unit": "iter/sec",
            "range": "stddev: 0.000001767397290553857",
            "extra": "mean: 42.79724925879344 usec\nrounds: 22936"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1610947.2025962332,
            "unit": "iter/sec",
            "range": "stddev: 1.6131938522938654e-7",
            "extra": "mean: 620.7528082785309 nsec\nrounds: 163908"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 37708.15648024797,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021006381620922686",
            "extra": "mean: 26.51946139355323 usec\nrounds: 33427"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 739.7925102322151,
            "unit": "iter/sec",
            "range": "stddev: 0.000023370105166738404",
            "extra": "mean: 1.351730365161588 msec\nrounds: 775"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 603.9273065494483,
            "unit": "iter/sec",
            "range": "stddev: 0.000024463168208180213",
            "extra": "mean: 1.6558284236450933 msec\nrounds: 609"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1683121.4634752104,
            "unit": "iter/sec",
            "range": "stddev: 4.3511429122714804e-8",
            "extra": "mean: 594.1341856191144 nsec\nrounds: 168919"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 38856.18346791175,
            "unit": "iter/sec",
            "range": "stddev: 0.000002866204342435636",
            "extra": "mean: 25.735929541969064 usec\nrounds: 33779"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 972961.8616415208,
            "unit": "iter/sec",
            "range": "stddev: 6.974355911398347e-8",
            "extra": "mean: 1.0277895151129923 usec\nrounds: 93897"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17017.83436131648,
            "unit": "iter/sec",
            "range": "stddev: 0.00000258210865180076",
            "extra": "mean: 58.761883490481985 usec\nrounds: 16548"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1182612.8730693315,
            "unit": "iter/sec",
            "range": "stddev: 6.686074805840651e-8",
            "extra": "mean: 845.5852483701051 nsec\nrounds: 111025"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 17589.837420838267,
            "unit": "iter/sec",
            "range": "stddev: 0.000003301109466201675",
            "extra": "mean: 56.85100868614758 usec\nrounds: 17154"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1083728.8283837324,
            "unit": "iter/sec",
            "range": "stddev: 5.973533022124984e-8",
            "extra": "mean: 922.7400561923221 nsec\nrounds: 53923"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 15853.037596238768,
            "unit": "iter/sec",
            "range": "stddev: 0.000003584940155052",
            "extra": "mean: 63.07939370794504 usec\nrounds: 15575"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1811520.2253722167,
            "unit": "iter/sec",
            "range": "stddev: 6.399950436286986e-8",
            "extra": "mean: 552.0225421687793 nsec\nrounds: 173883"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 51631.97914642749,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015289086784354129",
            "extra": "mean: 19.367841723905556 usec\nrounds: 40840"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 224043.0104329139,
            "unit": "iter/sec",
            "range": "stddev: 3.925386180891498e-7",
            "extra": "mean: 4.4634286874994205 usec\nrounds: 195695"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2590.975560483015,
            "unit": "iter/sec",
            "range": "stddev: 0.000007489704323549576",
            "extra": "mean: 385.9550106345186 usec\nrounds: 2633"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 160184.25640712024,
            "unit": "iter/sec",
            "range": "stddev: 8.679653026412301e-7",
            "extra": "mean: 6.242810763240211 usec\nrounds: 142187"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2034.5567347202004,
            "unit": "iter/sec",
            "range": "stddev: 0.000027395611059031304",
            "extra": "mean: 491.507551957023 usec\nrounds: 2069"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 55869.88877659699,
            "unit": "iter/sec",
            "range": "stddev: 0.000001075811418514435",
            "extra": "mean: 17.898729027341183 usec\nrounds: 49636"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 618.6222009225563,
            "unit": "iter/sec",
            "range": "stddev: 0.000012106332097810417",
            "extra": "mean: 1.6164954935479066 msec\nrounds: 620"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 128992.72187862828,
            "unit": "iter/sec",
            "range": "stddev: 6.740654934519365e-7",
            "extra": "mean: 7.752375369991178 usec\nrounds: 104734"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 152794.9018684989,
            "unit": "iter/sec",
            "range": "stddev: 6.401478237427444e-7",
            "extra": "mean: 6.544720980681921 usec\nrounds: 128966"
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
          "id": "8624f1a830a788c51c318cf00d5e3a9111d50647",
          "message": "Use inline shift for APyFixed",
          "timestamp": "2024-03-18T12:24:47+01:00",
          "tree_id": "ce60e8d72d0599048f0325a173b54ba40c955da0",
          "url": "https://github.com/apytypes/apytypes/commit/8624f1a830a788c51c318cf00d5e3a9111d50647"
        },
        "date": 1710761277557,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25246.108611672087,
            "unit": "iter/sec",
            "range": "stddev: 0.0000019173231551037574",
            "extra": "mean: 39.61006487699526 usec\nrounds: 24585"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1824069.4800789228,
            "unit": "iter/sec",
            "range": "stddev: 5.597550376822257e-8",
            "extra": "mean: 548.22473097735 nsec\nrounds: 181489"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 45702.668592861155,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018867663102244915",
            "extra": "mean: 21.880560387149952 usec\nrounds: 34312"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 742.8397574054018,
            "unit": "iter/sec",
            "range": "stddev: 0.000017784648869193613",
            "extra": "mean: 1.3461853515929332 msec\nrounds: 785"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 618.1617396407456,
            "unit": "iter/sec",
            "range": "stddev: 0.000025590684440156963",
            "extra": "mean: 1.6176996016951253 msec\nrounds: 590"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1795756.2846553312,
            "unit": "iter/sec",
            "range": "stddev: 5.3986395698496325e-8",
            "extra": "mean: 556.8684395232517 nsec\nrounds: 173581"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 47138.99106897845,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018032493607615782",
            "extra": "mean: 21.213860910529476 usec\nrounds: 35035"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 910331.2565776434,
            "unit": "iter/sec",
            "range": "stddev: 6.80414415666529e-8",
            "extra": "mean: 1.0985012244437415 usec\nrounds: 90654"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 15409.248094094663,
            "unit": "iter/sec",
            "range": "stddev: 0.0000033677827821239114",
            "extra": "mean: 64.89609317038858 usec\nrounds: 15037"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1120529.1856864686,
            "unit": "iter/sec",
            "range": "stddev: 6.165004364221577e-8",
            "extra": "mean: 892.4354784988709 nsec\nrounds: 111025"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 17653.13445158893,
            "unit": "iter/sec",
            "range": "stddev: 0.000003587539681270634",
            "extra": "mean: 56.64716386443155 usec\nrounds: 15879"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 997788.5721957624,
            "unit": "iter/sec",
            "range": "stddev: 5.5286402485541155e-8",
            "extra": "mean: 1.0022163290559374 usec\nrounds: 50058"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 15984.494515488323,
            "unit": "iter/sec",
            "range": "stddev: 0.000004304981248725637",
            "extra": "mean: 62.560627052112324 usec\nrounds: 14742"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1840570.4261449995,
            "unit": "iter/sec",
            "range": "stddev: 4.584168491796399e-8",
            "extra": "mean: 543.3098271030715 nsec\nrounds: 170620"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 51825.785987290205,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016189031942677444",
            "extra": "mean: 19.2954140675308 usec\nrounds: 48736"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 228815.96778250716,
            "unit": "iter/sec",
            "range": "stddev: 3.784565783769339e-7",
            "extra": "mean: 4.3703243689291575 usec\nrounds: 114732"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2636.204776214989,
            "unit": "iter/sec",
            "range": "stddev: 0.000014650118706133036",
            "extra": "mean: 379.3332024213158 usec\nrounds: 2643"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 161835.76700715552,
            "unit": "iter/sec",
            "range": "stddev: 5.324538281301518e-7",
            "extra": "mean: 6.179103782143445 usec\nrounds: 140371"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2030.8904390095634,
            "unit": "iter/sec",
            "range": "stddev: 0.000017697882433959552",
            "extra": "mean: 492.39485340611776 usec\nrounds: 2026"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 62328.59839430325,
            "unit": "iter/sec",
            "range": "stddev: 9.96311885970878e-7",
            "extra": "mean: 16.043999476352713 usec\nrounds: 55390"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 668.4003332689779,
            "unit": "iter/sec",
            "range": "stddev: 0.000051862466435564034",
            "extra": "mean: 1.4961093677336328 msec\nrounds: 688"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 144371.42381717285,
            "unit": "iter/sec",
            "range": "stddev: 7.408180005894957e-7",
            "extra": "mean: 6.926578498431701 usec\nrounds: 105397"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 163647.76474436192,
            "unit": "iter/sec",
            "range": "stddev: 4.902726035321063e-7",
            "extra": "mean: 6.11068535865506 usec\nrounds: 137118"
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
          "id": "1cfd03b3ce602906dafa7e688b2d03ef70f9f403",
          "message": "Reuse variables for floating-point array arithmetic",
          "timestamp": "2024-03-18T12:45:51+01:00",
          "tree_id": "a7bcef98986f1516c9269c25df3071bc76bb461b",
          "url": "https://github.com/apytypes/apytypes/commit/1cfd03b3ce602906dafa7e688b2d03ef70f9f403"
        },
        "date": 1710762534137,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24892.161680965404,
            "unit": "iter/sec",
            "range": "stddev: 0.0000017290530539409682",
            "extra": "mean: 40.173288797359945 usec\nrounds: 24280"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1777451.521974625,
            "unit": "iter/sec",
            "range": "stddev: 5.674716108961014e-8",
            "extra": "mean: 562.6032483232473 nsec\nrounds: 174490"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 46947.26494173015,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015269990306735128",
            "extra": "mean: 21.300495380107378 usec\nrounds: 38096"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 758.5195566322508,
            "unit": "iter/sec",
            "range": "stddev: 0.00002677865509398219",
            "extra": "mean: 1.3183575706866382 msec\nrounds: 771"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 628.5740618836394,
            "unit": "iter/sec",
            "range": "stddev: 0.00002672844944223872",
            "extra": "mean: 1.5909024260455698 msec\nrounds: 622"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1830592.3211380953,
            "unit": "iter/sec",
            "range": "stddev: 4.068069155573342e-8",
            "extra": "mean: 546.2712743044652 nsec\nrounds: 176960"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 48217.07261019068,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015241256795662106",
            "extra": "mean: 20.739541947817255 usec\nrounds: 32898"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 878927.9438214004,
            "unit": "iter/sec",
            "range": "stddev: 9.263230193621004e-8",
            "extra": "mean: 1.1377496949888821 usec\nrounds: 87866"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 15442.568514754614,
            "unit": "iter/sec",
            "range": "stddev: 0.000003209053932330466",
            "extra": "mean: 64.75606690975981 usec\nrounds: 15349"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1093480.3251723452,
            "unit": "iter/sec",
            "range": "stddev: 5.746062884468522e-8",
            "extra": "mean: 914.5111960219904 nsec\nrounds: 112272"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 17970.615955023597,
            "unit": "iter/sec",
            "range": "stddev: 0.0000028815663970269306",
            "extra": "mean: 55.6463953435305 usec\nrounds: 17524"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 974470.6147620803,
            "unit": "iter/sec",
            "range": "stddev: 4.546717492066263e-8",
            "extra": "mean: 1.026198209418708 usec\nrounds: 48057"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 16170.840634931747,
            "unit": "iter/sec",
            "range": "stddev: 0.000003594082270218012",
            "extra": "mean: 61.83970410541497 usec\nrounds: 15759"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1644856.3003298817,
            "unit": "iter/sec",
            "range": "stddev: 5.261702480946951e-8",
            "extra": "mean: 607.955843801964 nsec\nrounds: 171498"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 53049.281456276614,
            "unit": "iter/sec",
            "range": "stddev: 0.0000013732699459154389",
            "extra": "mean: 18.850396698100486 usec\nrounds: 48034"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 229032.5927507648,
            "unit": "iter/sec",
            "range": "stddev: 3.104821620171036e-7",
            "extra": "mean: 4.366190802757092 usec\nrounds: 114469"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2647.2431318849926,
            "unit": "iter/sec",
            "range": "stddev: 0.0000055194638890445734",
            "extra": "mean: 377.7514758487413 usec\nrounds: 2650"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 161078.42293551227,
            "unit": "iter/sec",
            "range": "stddev: 4.847314352392827e-7",
            "extra": "mean: 6.2081561377115655 usec\nrounds: 145519"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2036.5776741520701,
            "unit": "iter/sec",
            "range": "stddev: 0.000005275213965867795",
            "extra": "mean: 491.0198185376605 usec\nrounds: 2039"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 62168.63676726943,
            "unit": "iter/sec",
            "range": "stddev: 0.0000014111484339836547",
            "extra": "mean: 16.085281132084923 usec\nrounds: 54903"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 692.677764090932,
            "unit": "iter/sec",
            "range": "stddev: 0.000011875774381073968",
            "extra": "mean: 1.4436727318833409 msec\nrounds: 690"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 137817.9774115066,
            "unit": "iter/sec",
            "range": "stddev: 6.073746570346389e-7",
            "extra": "mean: 7.2559474372064665 usec\nrounds: 106406"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 159976.9027329982,
            "unit": "iter/sec",
            "range": "stddev: 5.28622802800689e-7",
            "extra": "mean: 6.250902367256117 usec\nrounds: 128123"
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
          "id": "ce8522b7e8c05ebc181b8b92a8314f805a068c34",
          "message": "Add fast path for floating-point multiplication",
          "timestamp": "2024-03-18T13:53:20+01:00",
          "tree_id": "e43a5b86300e1757e76647e397115aae9b8e8b56",
          "url": "https://github.com/apytypes/apytypes/commit/ce8522b7e8c05ebc181b8b92a8314f805a068c34"
        },
        "date": 1710766594570,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25146.501207812318,
            "unit": "iter/sec",
            "range": "stddev: 0.000003324843946815236",
            "extra": "mean: 39.76696367164303 usec\nrounds: 24664"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1812060.7188842222,
            "unit": "iter/sec",
            "range": "stddev: 5.62828989928245e-8",
            "extra": "mean: 551.8578873095702 nsec\nrounds: 178540"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 47759.785903797834,
            "unit": "iter/sec",
            "range": "stddev: 0.000001751795509391557",
            "extra": "mean: 20.938117310958894 usec\nrounds: 26596"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 691.5758322746238,
            "unit": "iter/sec",
            "range": "stddev: 0.000023779260745032533",
            "extra": "mean: 1.4459730275868017 msec\nrounds: 725"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 623.5664655047733,
            "unit": "iter/sec",
            "range": "stddev: 0.000039279511482903565",
            "extra": "mean: 1.603678285025328 msec\nrounds: 621"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1874494.8274681882,
            "unit": "iter/sec",
            "range": "stddev: 4.1713387799436444e-8",
            "extra": "mean: 533.4770655786152 nsec\nrounds: 185529"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 45985.00127159142,
            "unit": "iter/sec",
            "range": "stddev: 0.0000030416563343367056",
            "extra": "mean: 21.746220992664828 usec\nrounds: 25752"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 914185.6786661998,
            "unit": "iter/sec",
            "range": "stddev: 6.229924551511895e-8",
            "extra": "mean: 1.0938696846126108 usec\nrounds: 90901"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 15437.59325004202,
            "unit": "iter/sec",
            "range": "stddev: 0.000002268869132894093",
            "extra": "mean: 64.7769366508784 usec\nrounds: 14207"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1099039.764978662,
            "unit": "iter/sec",
            "range": "stddev: 6.556918841288767e-8",
            "extra": "mean: 909.8851851093257 nsec\nrounds: 108850"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 17641.900780968168,
            "unit": "iter/sec",
            "range": "stddev: 0.000002481825263075818",
            "extra": "mean: 56.683234557060075 usec\nrounds: 16043"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 985098.5084849389,
            "unit": "iter/sec",
            "range": "stddev: 8.291884754388527e-8",
            "extra": "mean: 1.0151269049609837 usec\nrounds: 49437"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 16045.258907952975,
            "unit": "iter/sec",
            "range": "stddev: 0.0000029767780502032958",
            "extra": "mean: 62.323706070229946 usec\nrounds: 15650"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1599004.5349286725,
            "unit": "iter/sec",
            "range": "stddev: 4.902984347190109e-8",
            "extra": "mean: 625.3890956254266 nsec\nrounds: 154250"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 52679.58545154907,
            "unit": "iter/sec",
            "range": "stddev: 0.000001502526300048857",
            "extra": "mean: 18.982685445004662 usec\nrounds: 44679"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 221288.1880340744,
            "unit": "iter/sec",
            "range": "stddev: 4.170010063305499e-7",
            "extra": "mean: 4.518994027128181 usec\nrounds: 194213"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2640.3142645839234,
            "unit": "iter/sec",
            "range": "stddev: 0.000005117960997890661",
            "extra": "mean: 378.74279339152304 usec\nrounds: 2633"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 163074.5658698127,
            "unit": "iter/sec",
            "range": "stddev: 5.009471876220274e-7",
            "extra": "mean: 6.1321641095051564 usec\nrounds: 139998"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2031.5587863361934,
            "unit": "iter/sec",
            "range": "stddev: 0.000006015885684661221",
            "extra": "mean: 492.23286410699734 usec\nrounds: 2031"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 62250.455703679196,
            "unit": "iter/sec",
            "range": "stddev: 0.0000010550521040408136",
            "extra": "mean: 16.064139429920623 usec\nrounds: 55024"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 692.0310148774647,
            "unit": "iter/sec",
            "range": "stddev: 0.000013313058341092056",
            "extra": "mean: 1.445021940493615 msec\nrounds: 689"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 142211.32378124428,
            "unit": "iter/sec",
            "range": "stddev: 6.25396890424849e-7",
            "extra": "mean: 7.03178884361026 usec\nrounds: 102166"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 157018.67664733,
            "unit": "iter/sec",
            "range": "stddev: 4.6618032837176057e-7",
            "extra": "mean: 6.368669137659583 usec\nrounds: 130311"
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
          "id": "4c6ca2566921c2fc9b46840887dee88ec93197c4",
          "message": "Add unary negation and specialize array/scalar operations",
          "timestamp": "2024-03-18T17:30:20+01:00",
          "tree_id": "cb4c776d6cce7c51122365014a72eeb43deb5473",
          "url": "https://github.com/apytypes/apytypes/commit/4c6ca2566921c2fc9b46840887dee88ec93197c4"
        },
        "date": 1710779607128,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24890.840380870446,
            "unit": "iter/sec",
            "range": "stddev: 0.000002407663862024863",
            "extra": "mean: 40.175421347707406 usec\nrounds: 24227"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1795865.2955576328,
            "unit": "iter/sec",
            "range": "stddev: 1.2622370152558424e-7",
            "extra": "mean: 556.8346370263672 nsec\nrounds: 177905"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 45626.57429109323,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021624924954101143",
            "extra": "mean: 21.917051971074894 usec\nrounds: 30921"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 748.9996517216188,
            "unit": "iter/sec",
            "range": "stddev: 0.000022775083399475444",
            "extra": "mean: 1.335114105462456 msec\nrounds: 787"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 620.431631243074,
            "unit": "iter/sec",
            "range": "stddev: 0.00003702155053735184",
            "extra": "mean: 1.6117811369424166 msec\nrounds: 628"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1861287.1126467634,
            "unit": "iter/sec",
            "range": "stddev: 4.612286844042055e-8",
            "extra": "mean: 537.2626249895881 nsec\nrounds: 180832"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 47734.49899032179,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016905631840198996",
            "extra": "mean: 20.94920908676031 usec\nrounds: 39113"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 965703.0598575074,
            "unit": "iter/sec",
            "range": "stddev: 6.10088253182208e-8",
            "extra": "mean: 1.035514995828582 usec\nrounds: 93546"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 16946.87147245406,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021535620571417102",
            "extra": "mean: 59.0079414731757 usec\nrounds: 16830"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1169061.005197077,
            "unit": "iter/sec",
            "range": "stddev: 5.936881135283948e-8",
            "extra": "mean: 855.387354085518 nsec\nrounds: 111285"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 20123.86571676341,
            "unit": "iter/sec",
            "range": "stddev: 0.000002158278535286536",
            "extra": "mean: 49.692241742946464 usec\nrounds: 18106"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1048285.0092672432,
            "unit": "iter/sec",
            "range": "stddev: 4.562275327804674e-8",
            "extra": "mean: 953.9390444007248 nsec\nrounds: 49906"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 16193.864576502578,
            "unit": "iter/sec",
            "range": "stddev: 0.0000026286543043739733",
            "extra": "mean: 61.75178230470123 usec\nrounds: 16027"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1521513.610727044,
            "unit": "iter/sec",
            "range": "stddev: 5.069173348706473e-8",
            "extra": "mean: 657.2402592718915 nsec\nrounds: 148105"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 40385.262397780265,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016145499734162045",
            "extra": "mean: 24.761508050891457 usec\nrounds: 38567"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 224396.36384943346,
            "unit": "iter/sec",
            "range": "stddev: 3.503064016498743e-7",
            "extra": "mean: 4.456400196711675 usec\nrounds: 197278"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2644.9962305696245,
            "unit": "iter/sec",
            "range": "stddev: 0.000004784995310313038",
            "extra": "mean: 378.07237244517387 usec\nrounds: 2642"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 168135.79290051913,
            "unit": "iter/sec",
            "range": "stddev: 4.33978960670906e-7",
            "extra": "mean: 5.94757358173979 usec\nrounds: 141985"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2088.3055393304858,
            "unit": "iter/sec",
            "range": "stddev: 0.00000666302976241804",
            "extra": "mean: 478.8571313758051 usec\nrounds: 2078"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 61293.892829833734,
            "unit": "iter/sec",
            "range": "stddev: 0.000001037148723533369",
            "extra": "mean: 16.31483911090841 usec\nrounds: 54696"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 681.4905310626181,
            "unit": "iter/sec",
            "range": "stddev: 0.000009355130911314581",
            "extra": "mean: 1.4673718187114708 msec\nrounds: 684"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 140705.06386078824,
            "unit": "iter/sec",
            "range": "stddev: 5.128869970667615e-7",
            "extra": "mean: 7.107064753471751 usec\nrounds: 109446"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 164800.87163728048,
            "unit": "iter/sec",
            "range": "stddev: 4.4007666576706365e-7",
            "extra": "mean: 6.067929071400522 usec\nrounds: 137307"
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
          "id": "386af56e4182053c65559ed6fbe59e22eff8b257",
          "message": "Special casing for double to fixed-point conversion",
          "timestamp": "2024-03-18T17:48:54+01:00",
          "tree_id": "22585c10515b3eaa1b1e87d7ca88e3d502dda7f9",
          "url": "https://github.com/apytypes/apytypes/commit/386af56e4182053c65559ed6fbe59e22eff8b257"
        },
        "date": 1710780715075,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24677.485369435366,
            "unit": "iter/sec",
            "range": "stddev: 0.0000029435797922031863",
            "extra": "mean: 40.52276741448557 usec\nrounds: 23845"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1810373.0971365569,
            "unit": "iter/sec",
            "range": "stddev: 5.3228088046681104e-8",
            "extra": "mean: 552.3723267771247 nsec\nrounds: 177305"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 42625.76367429722,
            "unit": "iter/sec",
            "range": "stddev: 0.0000014063720641155549",
            "extra": "mean: 23.45999024535922 usec\nrounds: 30960"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 744.2776879440586,
            "unit": "iter/sec",
            "range": "stddev: 0.000013955447821575904",
            "extra": "mean: 1.3435845467332646 msec\nrounds: 781"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 615.4133459562055,
            "unit": "iter/sec",
            "range": "stddev: 0.00004914696683841845",
            "extra": "mean: 1.62492413687623 msec\nrounds: 621"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1808421.5819476778,
            "unit": "iter/sec",
            "range": "stddev: 7.29519933108444e-8",
            "extra": "mean: 552.9684062512979 nsec\nrounds: 182150"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 43972.063378759136,
            "unit": "iter/sec",
            "range": "stddev: 0.0000014922467369377752",
            "extra": "mean: 22.741711968036814 usec\nrounds: 36614"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 958582.6153972269,
            "unit": "iter/sec",
            "range": "stddev: 7.200714557001673e-8",
            "extra": "mean: 1.0432069014579466 usec\nrounds: 95603"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 16932.511711378258,
            "unit": "iter/sec",
            "range": "stddev: 0.0000030210833336389396",
            "extra": "mean: 59.05798366157473 usec\nrounds: 16403"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 523424.2515362307,
            "unit": "iter/sec",
            "range": "stddev: 1.6351565777336704e-7",
            "extra": "mean: 1.9104961168021055 usec\nrounds: 52122"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 17746.044976312845,
            "unit": "iter/sec",
            "range": "stddev: 0.0000034124742772845846",
            "extra": "mean: 56.35058410675646 usec\nrounds: 17341"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1020807.6518856985,
            "unit": "iter/sec",
            "range": "stddev: 4.130653942221105e-8",
            "extra": "mean: 979.616481276113 nsec\nrounds: 47827"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 16048.18488147862,
            "unit": "iter/sec",
            "range": "stddev: 0.0000035571558970034034",
            "extra": "mean: 62.31234294627989 usec\nrounds: 15396"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1764089.1426095054,
            "unit": "iter/sec",
            "range": "stddev: 4.204728021244742e-8",
            "extra": "mean: 566.8647778880244 nsec\nrounds: 155232"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 50557.786332933276,
            "unit": "iter/sec",
            "range": "stddev: 0.000001406877986788455",
            "extra": "mean: 19.779347011255542 usec\nrounds: 48010"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 221080.68341558313,
            "unit": "iter/sec",
            "range": "stddev: 2.888356669674951e-7",
            "extra": "mean: 4.523235519949156 usec\nrounds: 109806"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2557.142174222895,
            "unit": "iter/sec",
            "range": "stddev: 0.000013645040650338177",
            "extra": "mean: 391.06155695230206 usec\nrounds: 2546"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 183245.63872891178,
            "unit": "iter/sec",
            "range": "stddev: 4.946601205578464e-7",
            "extra": "mean: 5.457155798831156 usec\nrounds: 162023"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2402.368153150748,
            "unit": "iter/sec",
            "range": "stddev: 0.000006155227383765818",
            "extra": "mean: 416.25593424907936 usec\nrounds: 2403"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 62012.4438863048,
            "unit": "iter/sec",
            "range": "stddev: 0.000001120242966378995",
            "extra": "mean: 16.125795684385952 usec\nrounds: 54455"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 698.3080833112773,
            "unit": "iter/sec",
            "range": "stddev: 0.000014701139631402945",
            "extra": "mean: 1.432032685713937 msec\nrounds: 700"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 139217.08048311694,
            "unit": "iter/sec",
            "range": "stddev: 6.103565333125968e-7",
            "extra": "mean: 7.18302665542014 usec\nrounds: 104406"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 162176.97880674715,
            "unit": "iter/sec",
            "range": "stddev: 4.7439778491281773e-7",
            "extra": "mean: 6.166103274075767 usec\nrounds: 136743"
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
          "id": "6f1613718018f34beef7cc2801b3f740c395b1f8",
          "message": "src: fix memory out-of-bounds access bug, closes #179",
          "timestamp": "2024-03-18T18:18:25+01:00",
          "tree_id": "df4d00301812fd2d2eb9f063f7b2e3de1146007a",
          "url": "https://github.com/apytypes/apytypes/commit/6f1613718018f34beef7cc2801b3f740c395b1f8"
        },
        "date": 1710782887640,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25011.22770824563,
            "unit": "iter/sec",
            "range": "stddev: 0.000001201417480378277",
            "extra": "mean: 39.98204373111692 usec\nrounds: 24239"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1792566.8801411062,
            "unit": "iter/sec",
            "range": "stddev: 5.2062184270191293e-8",
            "extra": "mean: 557.8592414478767 nsec\nrounds: 176336"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 41787.763033351315,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018252848761536536",
            "extra": "mean: 23.93045062502839 usec\nrounds: 33205"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 652.4761995562277,
            "unit": "iter/sec",
            "range": "stddev: 0.00002387902894914442",
            "extra": "mean: 1.5326229534198115 msec\nrounds: 687"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 618.4634763937838,
            "unit": "iter/sec",
            "range": "stddev: 0.000058100744805770514",
            "extra": "mean: 1.6169103563413774 msec\nrounds: 623"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1797426.4331946073,
            "unit": "iter/sec",
            "range": "stddev: 4.1166930662486396e-8",
            "extra": "mean: 556.3510035972487 nsec\nrounds: 175717"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 46197.55597616138,
            "unit": "iter/sec",
            "range": "stddev: 0.0000017240165034478697",
            "extra": "mean: 21.64616674778239 usec\nrounds: 39005"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 920793.263188104,
            "unit": "iter/sec",
            "range": "stddev: 6.682459175804997e-8",
            "extra": "mean: 1.0860201089413266 usec\nrounds: 92337"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 16893.77617270765,
            "unit": "iter/sec",
            "range": "stddev: 0.000004818547448839752",
            "extra": "mean: 59.193397010641526 usec\nrounds: 16793"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1183847.4943488992,
            "unit": "iter/sec",
            "range": "stddev: 5.45033892061102e-8",
            "extra": "mean: 844.7033969945568 nsec\nrounds: 118134"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 17651.028767750908,
            "unit": "iter/sec",
            "range": "stddev: 0.000005750625577330452",
            "extra": "mean: 56.653921601841 usec\nrounds: 17054"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 934914.6460699544,
            "unit": "iter/sec",
            "range": "stddev: 2.1928521498027444e-7",
            "extra": "mean: 1.0696163593154 usec\nrounds: 49510"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 16041.346546755669,
            "unit": "iter/sec",
            "range": "stddev: 0.0000023934123928953963",
            "extra": "mean: 62.338906343386 usec\nrounds: 14692"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1647725.4858435146,
            "unit": "iter/sec",
            "range": "stddev: 4.3421130313749385e-8",
            "extra": "mean: 606.8972098759804 nsec\nrounds: 171498"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 51653.89210959562,
            "unit": "iter/sec",
            "range": "stddev: 0.000001218562360697471",
            "extra": "mean: 19.35962536720892 usec\nrounds: 39113"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 219273.3737883999,
            "unit": "iter/sec",
            "range": "stddev: 3.281334871251796e-7",
            "extra": "mean: 4.5605172334558315 usec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2560.535723696008,
            "unit": "iter/sec",
            "range": "stddev: 0.000013563736995622867",
            "extra": "mean: 390.54327215421506 usec\nrounds: 2539"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 188445.1933935475,
            "unit": "iter/sec",
            "range": "stddev: 3.7914919705375335e-7",
            "extra": "mean: 5.3065826832293235 usec\nrounds: 158932"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 2402.3492875102,
            "unit": "iter/sec",
            "range": "stddev: 0.000005872208776153838",
            "extra": "mean: 416.25920310547434 usec\nrounds: 2383"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 56946.52567472795,
            "unit": "iter/sec",
            "range": "stddev: 7.524823810317359e-7",
            "extra": "mean: 17.560333807050597 usec\nrounds: 50667"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 629.3864382772491,
            "unit": "iter/sec",
            "range": "stddev: 0.000011993186898600876",
            "extra": "mean: 1.5888489792331573 msec\nrounds: 626"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 128851.70244483829,
            "unit": "iter/sec",
            "range": "stddev: 5.194278704251008e-7",
            "extra": "mean: 7.76085981811612 usec\nrounds: 102902"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 161891.07499181278,
            "unit": "iter/sec",
            "range": "stddev: 5.653366493448941e-7",
            "extra": "mean: 6.176992771532169 usec\nrounds: 133085"
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
          "id": "12af9d17ddfa5ee0d8bd3076a5cbdcd2a2b13d31",
          "message": "Add optmized two's overflow for single limb values",
          "timestamp": "2024-03-19T10:12:29+01:00",
          "tree_id": "5ecc3fa640bf11351eec27c66bf59994e2f9e629",
          "url": "https://github.com/apytypes/apytypes/commit/12af9d17ddfa5ee0d8bd3076a5cbdcd2a2b13d31"
        },
        "date": 1710839735213,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25056.790832233306,
            "unit": "iter/sec",
            "range": "stddev: 0.0000012794440690983141",
            "extra": "mean: 39.90934061330751 usec\nrounds: 24268"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1842706.013958185,
            "unit": "iter/sec",
            "range": "stddev: 5.232059531399372e-8",
            "extra": "mean: 542.6801629913098 nsec\nrounds: 180506"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 46962.90496845946,
            "unit": "iter/sec",
            "range": "stddev: 0.0000014736167863798797",
            "extra": "mean: 21.293401689516553 usec\nrounds: 39894"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 759.6117417337532,
            "unit": "iter/sec",
            "range": "stddev: 0.00002347690038563007",
            "extra": "mean: 1.31646200954922 msec\nrounds: 733"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 622.6044885649088,
            "unit": "iter/sec",
            "range": "stddev: 0.00007469932091750818",
            "extra": "mean: 1.6061561045038084 msec\nrounds: 622"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1809295.2855220055,
            "unit": "iter/sec",
            "range": "stddev: 4.9135661832783774e-8",
            "extra": "mean: 552.7013793723984 nsec\nrounds: 179212"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 47839.25535525984,
            "unit": "iter/sec",
            "range": "stddev: 0.000001525144430759241",
            "extra": "mean: 20.903335400475708 usec\nrounds: 39991"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 964724.1223383095,
            "unit": "iter/sec",
            "range": "stddev: 6.916170715010349e-8",
            "extra": "mean: 1.036565767191736 usec\nrounds: 95239"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 16980.022781883086,
            "unit": "iter/sec",
            "range": "stddev: 0.0000020748851365336406",
            "extra": "mean: 58.8927360608111 usec\nrounds: 16572"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1096120.741496461,
            "unit": "iter/sec",
            "range": "stddev: 5.974020113081948e-8",
            "extra": "mean: 912.3082541388819 nsec\nrounds: 103972"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 17879.45822872292,
            "unit": "iter/sec",
            "range": "stddev: 0.0000017318163748265032",
            "extra": "mean: 55.930106338094966 usec\nrounds: 17623"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1303127.0096543056,
            "unit": "iter/sec",
            "range": "stddev: 5.948918776169902e-8",
            "extra": "mean: 767.3849076808382 nsec\nrounds: 129450"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 20728.795510247757,
            "unit": "iter/sec",
            "range": "stddev: 0.0000020237723862547473",
            "extra": "mean: 48.242069806015834 usec\nrounds: 19812"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1644198.8538357033,
            "unit": "iter/sec",
            "range": "stddev: 2.4141582307258616e-7",
            "extra": "mean: 608.1989399682817 nsec\nrounds: 158203"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 53871.00050273363,
            "unit": "iter/sec",
            "range": "stddev: 0.0000013147878551560031",
            "extra": "mean: 18.5628629627782 usec\nrounds: 49461"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 225160.72977192292,
            "unit": "iter/sec",
            "range": "stddev: 3.530758379689248e-7",
            "extra": "mean: 4.441271801761134 usec\nrounds: 198413"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2603.501767884678,
            "unit": "iter/sec",
            "range": "stddev: 0.000008431169847002583",
            "extra": "mean: 384.09806835372 usec\nrounds: 2648"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 244157.25541326642,
            "unit": "iter/sec",
            "range": "stddev: 3.846056691879794e-7",
            "extra": "mean: 4.095721006968955 usec\nrounds: 198453"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3397.3268101722138,
            "unit": "iter/sec",
            "range": "stddev: 0.000022318253285610722",
            "extra": "mean: 294.34907380880117 usec\nrounds: 3211"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 56023.43485766942,
            "unit": "iter/sec",
            "range": "stddev: 9.608370672771817e-7",
            "extra": "mean: 17.849673133047883 usec\nrounds: 50106"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 613.1339720308354,
            "unit": "iter/sec",
            "range": "stddev: 0.00007666395738380129",
            "extra": "mean: 1.6309649205829824 msec\nrounds: 617"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 138397.66862575457,
            "unit": "iter/sec",
            "range": "stddev: 5.207019873736064e-7",
            "extra": "mean: 7.225555241859826 usec\nrounds: 108378"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 167797.59142177834,
            "unit": "iter/sec",
            "range": "stddev: 4.6843391963503833e-7",
            "extra": "mean: 5.959561108874239 usec\nrounds: 137476"
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
          "id": "e0905d02079c6768e085064ac9a7b9292781ede5",
          "message": "Increase performance for fixed-point array negation",
          "timestamp": "2024-03-19T10:41:59+01:00",
          "tree_id": "537ea6009f257c3640ed92077a3a43073f449e4c",
          "url": "https://github.com/apytypes/apytypes/commit/e0905d02079c6768e085064ac9a7b9292781ede5"
        },
        "date": 1710841512222,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 23463.141914073676,
            "unit": "iter/sec",
            "range": "stddev: 0.0000019071525870475639",
            "extra": "mean: 42.62003800097119 usec\nrounds: 22973"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1677388.1187906417,
            "unit": "iter/sec",
            "range": "stddev: 5.534393386278696e-8",
            "extra": "mean: 596.1649476335066 nsec\nrounds: 166334"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 39312.642484639466,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018218261180520366",
            "extra": "mean: 25.437109713261517 usec\nrounds: 31637"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 752.2598077936125,
            "unit": "iter/sec",
            "range": "stddev: 0.000013836650582526595",
            "extra": "mean: 1.329327965737014 msec\nrounds: 788"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 617.5651170322934,
            "unit": "iter/sec",
            "range": "stddev: 0.000013189997122072441",
            "extra": "mean: 1.6192624428100728 msec\nrounds: 612"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1697175.5282799127,
            "unit": "iter/sec",
            "range": "stddev: 4.318486207645649e-8",
            "extra": "mean: 589.2142464565582 nsec\nrounds: 165536"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 40405.47931644059,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016913917240830957",
            "extra": "mean: 24.74911860761196 usec\nrounds: 36684"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 962683.4160330007,
            "unit": "iter/sec",
            "range": "stddev: 5.763371070671682e-8",
            "extra": "mean: 1.0387630900725164 usec\nrounds: 96535"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17258.242112835876,
            "unit": "iter/sec",
            "range": "stddev: 0.0000023215599251312936",
            "extra": "mean: 57.943328959109145 usec\nrounds: 16993"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1254468.5807793823,
            "unit": "iter/sec",
            "range": "stddev: 5.279428758076841e-8",
            "extra": "mean: 797.150295608652 nsec\nrounds: 118400"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22843.914795574703,
            "unit": "iter/sec",
            "range": "stddev: 0.000002034455377341766",
            "extra": "mean: 43.77533399808158 usec\nrounds: 22572"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1209499.3520207696,
            "unit": "iter/sec",
            "range": "stddev: 5.2428654326783956e-8",
            "extra": "mean: 826.7883718410861 nsec\nrounds: 119546"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 18263.57025058637,
            "unit": "iter/sec",
            "range": "stddev: 0.000002100280834175295",
            "extra": "mean: 54.75380696542036 usec\nrounds: 17888"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1789966.241699423,
            "unit": "iter/sec",
            "range": "stddev: 4.1260382308205573e-8",
            "extra": "mean: 558.6697540454747 nsec\nrounds: 168039"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 52396.85974658557,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015195397343151964",
            "extra": "mean: 19.08511320786099 usec\nrounds: 47850"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 226967.31158913267,
            "unit": "iter/sec",
            "range": "stddev: 2.777199110016044e-7",
            "extra": "mean: 4.405920804182802 usec\nrounds: 114078"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2645.3931817404577,
            "unit": "iter/sec",
            "range": "stddev: 0.000009676752752842896",
            "extra": "mean: 378.01564126738987 usec\nrounds: 2651"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 239569.6249191012,
            "unit": "iter/sec",
            "range": "stddev: 4.2006694515551087e-7",
            "extra": "mean: 4.174151878969147 usec\nrounds: 193799"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3402.983323859222,
            "unit": "iter/sec",
            "range": "stddev: 0.00002003798824772716",
            "extra": "mean: 293.85980030778694 usec\nrounds: 3245"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 55518.46907622257,
            "unit": "iter/sec",
            "range": "stddev: 0.0000012036197821537684",
            "extra": "mean: 18.01202404603551 usec\nrounds: 49364"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 614.4356122181598,
            "unit": "iter/sec",
            "range": "stddev: 0.000010818424904990828",
            "extra": "mean: 1.6275098319739687 msec\nrounds: 613"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 143544.6185879976,
            "unit": "iter/sec",
            "range": "stddev: 5.678662656009592e-7",
            "extra": "mean: 6.9664750224472325 usec\nrounds: 107216"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 157002.9887656957,
            "unit": "iter/sec",
            "range": "stddev: 5.152775890122609e-7",
            "extra": "mean: 6.3693055008803405 usec\nrounds: 130124"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "committer": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "distinct": false,
          "id": "8fdcf13ac465a6ef94850f84f0c80dc9839539e0",
          "message": "Fix exception in multiplication",
          "timestamp": "2024-03-20T09:05:57+01:00",
          "tree_id": "dab4b4d2366191e3f34569bb6e583ce6d6aafc79",
          "url": "https://github.com/apytypes/apytypes/commit/8fdcf13ac465a6ef94850f84f0c80dc9839539e0"
        },
        "date": 1710922514486,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24578.051525525694,
            "unit": "iter/sec",
            "range": "stddev: 0.000003881569088619194",
            "extra": "mean: 40.68670777101446 usec\nrounds: 15789"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1795915.995255519,
            "unit": "iter/sec",
            "range": "stddev: 5.800834389394303e-8",
            "extra": "mean: 556.8189172778531 nsec\nrounds: 176648"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 46640.75024610178,
            "unit": "iter/sec",
            "range": "stddev: 0.0000017637999429871785",
            "extra": "mean: 21.44047843834973 usec\nrounds: 31329"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 563.0902652799414,
            "unit": "iter/sec",
            "range": "stddev: 0.000015040746237475447",
            "extra": "mean: 1.7759142035652986 msec\nrounds: 673"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 625.095649010748,
            "unit": "iter/sec",
            "range": "stddev: 0.00003502770258376886",
            "extra": "mean: 1.5997551759999624 msec\nrounds: 625"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1928742.0801500503,
            "unit": "iter/sec",
            "range": "stddev: 4.629592708496879e-8",
            "extra": "mean: 518.4726409464296 nsec\nrounds: 184502"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 47600.20810734112,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018843847820458744",
            "extra": "mean: 21.00831151294432 usec\nrounds: 32920"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 926457.3634113669,
            "unit": "iter/sec",
            "range": "stddev: 7.208912086378417e-8",
            "extra": "mean: 1.0793804868880534 usec\nrounds: 98339"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 15468.560803847015,
            "unit": "iter/sec",
            "range": "stddev: 0.000002569377428081092",
            "extra": "mean: 64.6472553381502 usec\nrounds: 15548"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1264583.1672934967,
            "unit": "iter/sec",
            "range": "stddev: 6.750387938823449e-8",
            "extra": "mean: 790.7744036639426 nsec\nrounds: 118681"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22795.44595245732,
            "unit": "iter/sec",
            "range": "stddev: 0.000005732723173221097",
            "extra": "mean: 43.86841135223333 usec\nrounds: 22815"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1283481.3739452858,
            "unit": "iter/sec",
            "range": "stddev: 5.402848116889536e-8",
            "extra": "mean: 779.1309015465579 nsec\nrounds: 116334"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 20414.610998502354,
            "unit": "iter/sec",
            "range": "stddev: 0.0000031070076626177764",
            "extra": "mean: 48.98452388210392 usec\nrounds: 20308"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1643364.9944307508,
            "unit": "iter/sec",
            "range": "stddev: 4.875235138715064e-8",
            "extra": "mean: 608.507546034455 nsec\nrounds: 147667"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 52744.038521703864,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015708876739450096",
            "extra": "mean: 18.95948865554741 usec\nrounds: 39975"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 227135.60902925415,
            "unit": "iter/sec",
            "range": "stddev: 2.780429865587398e-7",
            "extra": "mean: 4.402656211740027 usec\nrounds: 109566"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2638.622860105174,
            "unit": "iter/sec",
            "range": "stddev: 0.000014732311014394891",
            "extra": "mean: 378.9855743007322 usec\nrounds: 2537"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 241764.00014630775,
            "unit": "iter/sec",
            "range": "stddev: 4.366582839836638e-7",
            "extra": "mean: 4.136265115545872 usec\nrounds: 188680"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3420.417287720371,
            "unit": "iter/sec",
            "range": "stddev: 0.0000050421754275127875",
            "extra": "mean: 292.3619885766853 usec\nrounds: 3414"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 62161.34318946626,
            "unit": "iter/sec",
            "range": "stddev: 0.0000011089979296110425",
            "extra": "mean: 16.087168466614763 usec\nrounds: 53150"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 697.9797219328249,
            "unit": "iter/sec",
            "range": "stddev: 0.00004557467641348946",
            "extra": "mean: 1.4327063789630297 msec\nrounds: 694"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 132458.5928440747,
            "unit": "iter/sec",
            "range": "stddev: 5.014141624344916e-7",
            "extra": "mean: 7.549529090779052 usec\nrounds: 101338"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 160479.0357460252,
            "unit": "iter/sec",
            "range": "stddev: 5.869629138710928e-7",
            "extra": "mean: 6.231343523166505 usec\nrounds: 130294"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "committer": {
            "email": "40732757+Theodor-Lindberg@users.noreply.github.com",
            "name": "Theodor-Lindberg",
            "username": "Theodor-Lindberg"
          },
          "distinct": true,
          "id": "945083a637e09d72f4d413cef4e83360f300d646",
          "message": "Handle long formats in APyFloat power function",
          "timestamp": "2024-03-20T10:56:32+01:00",
          "tree_id": "d363a7dae42731cbd84aeadab196f860388baf8f",
          "url": "https://github.com/apytypes/apytypes/commit/945083a637e09d72f4d413cef4e83360f300d646"
        },
        "date": 1710928776402,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 23877.195022153206,
            "unit": "iter/sec",
            "range": "stddev: 0.000001674645201651597",
            "extra": "mean: 41.880966297431605 usec\nrounds: 23203"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1661865.8587501145,
            "unit": "iter/sec",
            "range": "stddev: 5.59809931355099e-8",
            "extra": "mean: 601.7332835467927 nsec\nrounds: 164718"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 39262.826538389185,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018122650664670774",
            "extra": "mean: 25.469383846378232 usec\nrounds: 31708"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 754.6662091882708,
            "unit": "iter/sec",
            "range": "stddev: 0.000014598418768584433",
            "extra": "mean: 1.3250891424907094 msec\nrounds: 779"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 619.8537111033588,
            "unit": "iter/sec",
            "range": "stddev: 0.000015042109117512587",
            "extra": "mean: 1.61328387986896 msec\nrounds: 616"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1645709.6695316816,
            "unit": "iter/sec",
            "range": "stddev: 5.3472883854629606e-8",
            "extra": "mean: 607.6405933038266 nsec\nrounds: 162549"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 39899.29683007659,
            "unit": "iter/sec",
            "range": "stddev: 0.0000019350725855261344",
            "extra": "mean: 25.063098336264098 usec\nrounds: 31738"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 973116.3644288507,
            "unit": "iter/sec",
            "range": "stddev: 6.170511903537096e-8",
            "extra": "mean: 1.0276263318076306 usec\nrounds: 100221"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17130.20307354061,
            "unit": "iter/sec",
            "range": "stddev: 0.000002335871830121319",
            "extra": "mean: 58.37642412684556 usec\nrounds: 17101"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1182730.0017176492,
            "unit": "iter/sec",
            "range": "stddev: 6.508733643440362e-8",
            "extra": "mean: 845.5015079923232 nsec\nrounds: 123077"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22677.46459937572,
            "unit": "iter/sec",
            "range": "stddev: 0.0000024007511863824003",
            "extra": "mean: 44.09664032845756 usec\nrounds: 21072"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1200924.6912464015,
            "unit": "iter/sec",
            "range": "stddev: 1.1802501621344505e-7",
            "extra": "mean: 832.6916810763776 nsec\nrounds: 116878"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 18051.134326308344,
            "unit": "iter/sec",
            "range": "stddev: 0.000003273101163961687",
            "extra": "mean: 55.39818063081862 usec\nrounds: 18070"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1783636.1160792524,
            "unit": "iter/sec",
            "range": "stddev: 4.1652078956192756e-8",
            "extra": "mean: 560.652473329694 nsec\nrounds: 169751"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 53143.98788513315,
            "unit": "iter/sec",
            "range": "stddev: 0.0000013441438581919822",
            "extra": "mean: 18.816803928252938 usec\nrounds: 48013"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 217221.55699771937,
            "unit": "iter/sec",
            "range": "stddev: 8.791659204731318e-7",
            "extra": "mean: 4.603594660775307 usec\nrounds: 195313"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2642.608681647633,
            "unit": "iter/sec",
            "range": "stddev: 0.00000944324199126806",
            "extra": "mean: 378.4139539632908 usec\nrounds: 1325"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 238271.1993181898,
            "unit": "iter/sec",
            "range": "stddev: 3.6319094931947887e-7",
            "extra": "mean: 4.196898336271811 usec\nrounds: 191976"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3416.7703628719682,
            "unit": "iter/sec",
            "range": "stddev: 0.000010371386166028378",
            "extra": "mean: 292.67404414016556 usec\nrounds: 3421"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 55473.64157856447,
            "unit": "iter/sec",
            "range": "stddev: 9.929770754230002e-7",
            "extra": "mean: 18.02657931846337 usec\nrounds: 50083"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 611.2464451980246,
            "unit": "iter/sec",
            "range": "stddev: 0.00008782011232278895",
            "extra": "mean: 1.6360013344143565 msec\nrounds: 616"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 139016.633313129,
            "unit": "iter/sec",
            "range": "stddev: 4.996045925826953e-7",
            "extra": "mean: 7.193383814349345 usec\nrounds: 106872"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 163713.9970267974,
            "unit": "iter/sec",
            "range": "stddev: 4.866547239573078e-7",
            "extra": "mean: 6.108213214269735 usec\nrounds: 126985"
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
          "id": "c478d297ca4154ef10258fa8f0e5949310cc8476",
          "message": "Restructure code for readability",
          "timestamp": "2024-03-20T12:11:04+01:00",
          "tree_id": "ab5740e9d48836c176f83823695ab1ea4f7b0e73",
          "url": "https://github.com/apytypes/apytypes/commit/c478d297ca4154ef10258fa8f0e5949310cc8476"
        },
        "date": 1710933249707,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24869.022900057927,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021018753405329824",
            "extra": "mean: 40.21066706234247 usec\nrounds: 22731"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1789226.1045936379,
            "unit": "iter/sec",
            "range": "stddev: 5.1748230805162813e-8",
            "extra": "mean: 558.9008551980342 nsec\nrounds: 177620"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 43915.303691276844,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016851724925549458",
            "extra": "mean: 22.77110519444355 usec\nrounds: 32920"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 745.277377078706,
            "unit": "iter/sec",
            "range": "stddev: 0.000017616293309004226",
            "extra": "mean: 1.3417823091849919 msec\nrounds: 773"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 618.0419308009348,
            "unit": "iter/sec",
            "range": "stddev: 0.00008922610646392309",
            "extra": "mean: 1.6180131964575233 msec\nrounds: 621"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1854279.3244460153,
            "unit": "iter/sec",
            "range": "stddev: 4.764802168277827e-8",
            "extra": "mean: 539.2930756529066 nsec\nrounds: 183151"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 44026.71148649098,
            "unit": "iter/sec",
            "range": "stddev: 0.000001757608701609755",
            "extra": "mean: 22.71348384279932 usec\nrounds: 38837"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 904785.7619893408,
            "unit": "iter/sec",
            "range": "stddev: 6.825238929820514e-8",
            "extra": "mean: 1.1052340145155723 usec\nrounds: 91067"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 15405.448561671441,
            "unit": "iter/sec",
            "range": "stddev: 0.000002279895627080657",
            "extra": "mean: 64.91209885884058 usec\nrounds: 15163"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1262893.8031350933,
            "unit": "iter/sec",
            "range": "stddev: 5.992019917023064e-8",
            "extra": "mean: 791.8322170221078 nsec\nrounds: 125235"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22208.6203895961,
            "unit": "iter/sec",
            "range": "stddev: 0.00000911813755054151",
            "extra": "mean: 45.02756058041598 usec\nrounds: 22598"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1266877.9520826526,
            "unit": "iter/sec",
            "range": "stddev: 5.4846946278617814e-8",
            "extra": "mean: 789.3420185867424 nsec\nrounds: 126663"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 20565.376176102258,
            "unit": "iter/sec",
            "range": "stddev: 0.0000029968820948211448",
            "extra": "mean: 48.62541737320797 usec\nrounds: 20169"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1627318.1674150582,
            "unit": "iter/sec",
            "range": "stddev: 4.3188912571655464e-8",
            "extra": "mean: 614.5079800764975 nsec\nrounds: 173883"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 51331.82202922845,
            "unit": "iter/sec",
            "range": "stddev: 0.000001513133930667857",
            "extra": "mean: 19.48109302316598 usec\nrounds: 48289"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 228250.05302594847,
            "unit": "iter/sec",
            "range": "stddev: 2.765383285957857e-7",
            "extra": "mean: 4.381159989856895 usec\nrounds: 114601"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2647.5746944942434,
            "unit": "iter/sec",
            "range": "stddev: 0.00001503835503492493",
            "extra": "mean: 377.70416905689086 usec\nrounds: 2650"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 241424.39228888394,
            "unit": "iter/sec",
            "range": "stddev: 4.068079778402324e-7",
            "extra": "mean: 4.142083533976213 usec\nrounds: 197668"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3392.4814355271956,
            "unit": "iter/sec",
            "range": "stddev: 0.000025697132209925247",
            "extra": "mean: 294.76948334268445 usec\nrounds: 3422"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 61811.43888786272,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015076944312167259",
            "extra": "mean: 16.178235258593208 usec\nrounds: 54693"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 698.1296543681416,
            "unit": "iter/sec",
            "range": "stddev: 0.00004663814667538002",
            "extra": "mean: 1.4323986866093996 msec\nrounds: 702"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 138571.63090210888,
            "unit": "iter/sec",
            "range": "stddev: 5.30291961580719e-7",
            "extra": "mean: 7.216484308439941 usec\nrounds: 106872"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 164845.8659658525,
            "unit": "iter/sec",
            "range": "stddev: 6.229281392328295e-7",
            "extra": "mean: 6.066272843063884 usec\nrounds: 135796"
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
          "id": "68cddb2248c5f525387429bbf6d8ab139744fdfe",
          "message": "Add test for long integers and APyFloat",
          "timestamp": "2024-03-20T12:41:56+01:00",
          "tree_id": "67d63d3b788f6fbeb6a96785d86e2fc9a357b73a",
          "url": "https://github.com/apytypes/apytypes/commit/68cddb2248c5f525387429bbf6d8ab139744fdfe"
        },
        "date": 1710935128233,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24930.007613486097,
            "unit": "iter/sec",
            "range": "stddev: 0.0000013732632943167832",
            "extra": "mean: 40.1123022304671 usec\nrounds: 22506"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1668962.1682443465,
            "unit": "iter/sec",
            "range": "stddev: 5.3775786193962606e-8",
            "extra": "mean: 599.1747560413531 nsec\nrounds: 164990"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 40013.42742736797,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016849152068923438",
            "extra": "mean: 24.991610674071634 usec\nrounds: 31778"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 557.6395646701766,
            "unit": "iter/sec",
            "range": "stddev: 0.00012447016978610868",
            "extra": "mean: 1.7932730447335876 msec\nrounds: 693"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 619.6123659993569,
            "unit": "iter/sec",
            "range": "stddev: 0.000029011173216661254",
            "extra": "mean: 1.6139122697900414 msec\nrounds: 619"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1708653.8863644055,
            "unit": "iter/sec",
            "range": "stddev: 8.256578949357795e-8",
            "extra": "mean: 585.256035748567 nsec\nrounds: 167477"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 37621.51312997532,
            "unit": "iter/sec",
            "range": "stddev: 0.000003598367495192404",
            "extra": "mean: 26.58053642194524 usec\nrounds: 20551"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1054988.9416175408,
            "unit": "iter/sec",
            "range": "stddev: 7.212076283659217e-8",
            "extra": "mean: 947.8772341127509 nsec\nrounds: 106519"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17139.039683834428,
            "unit": "iter/sec",
            "range": "stddev: 0.000002554892598870916",
            "extra": "mean: 58.346326191379426 usec\nrounds: 17036"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1145800.6866144082,
            "unit": "iter/sec",
            "range": "stddev: 6.627398355203553e-8",
            "extra": "mean: 872.75213890365 nsec\nrounds: 105731"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22747.848029581048,
            "unit": "iter/sec",
            "range": "stddev: 0.0000029177281222136075",
            "extra": "mean: 43.96020224416882 usec\nrounds: 21568"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1203321.5511402166,
            "unit": "iter/sec",
            "range": "stddev: 6.028764305764805e-8",
            "extra": "mean: 831.033067638788 nsec\nrounds: 110779"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 18349.218488178678,
            "unit": "iter/sec",
            "range": "stddev: 0.000002485911934328867",
            "extra": "mean: 54.49823384272421 usec\nrounds: 17995"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1786673.7496949092,
            "unit": "iter/sec",
            "range": "stddev: 4.462743883306239e-8",
            "extra": "mean: 559.6992736758815 nsec\nrounds: 149656"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 51008.21953548412,
            "unit": "iter/sec",
            "range": "stddev: 0.000001664733672083278",
            "extra": "mean: 19.604683502123514 usec\nrounds: 48667"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 223306.03674239913,
            "unit": "iter/sec",
            "range": "stddev: 4.06719907058891e-7",
            "extra": "mean: 4.478159276784702 usec\nrounds: 198020"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2646.658342104322,
            "unit": "iter/sec",
            "range": "stddev: 0.000005120069616117694",
            "extra": "mean: 377.8349415530958 usec\nrounds: 2652"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 248428.53969484323,
            "unit": "iter/sec",
            "range": "stddev: 2.666890707598979e-7",
            "extra": "mean: 4.025302411825743 usec\nrounds: 111907"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3420.7235561485395,
            "unit": "iter/sec",
            "range": "stddev: 0.0000043651442500504655",
            "extra": "mean: 292.3358124635829 usec\nrounds: 3418"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 56012.55834719853,
            "unit": "iter/sec",
            "range": "stddev: 9.579700345075717e-7",
            "extra": "mean: 17.85313917999275 usec\nrounds: 49145"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 614.7185979175662,
            "unit": "iter/sec",
            "range": "stddev: 0.000011665558265131672",
            "extra": "mean: 1.6267606078417365 msec\nrounds: 612"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 137297.40874740365,
            "unit": "iter/sec",
            "range": "stddev: 5.232752081105526e-7",
            "extra": "mean: 7.283458654633279 usec\nrounds: 105175"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 160161.8415020338,
            "unit": "iter/sec",
            "range": "stddev: 4.83402863658279e-7",
            "extra": "mean: 6.243684454560305 usec\nrounds: 132206"
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
          "id": "0209a95a24ff85505f46fe4b2038518b930485d7",
          "message": "Refactor left-shift to speed up cast",
          "timestamp": "2024-03-20T19:59:06+01:00",
          "tree_id": "72307b75fd30328d7199e24cae5e9c1ee3752721",
          "url": "https://github.com/apytypes/apytypes/commit/0209a95a24ff85505f46fe4b2038518b930485d7"
        },
        "date": 1710961341895,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25424.867744614905,
            "unit": "iter/sec",
            "range": "stddev: 0.000002435000582019779",
            "extra": "mean: 39.33157135937528 usec\nrounds: 24664"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1808595.2544082087,
            "unit": "iter/sec",
            "range": "stddev: 5.775106073330267e-8",
            "extra": "mean: 552.9153068176505 nsec\nrounds: 179534"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 45849.73651846105,
            "unit": "iter/sec",
            "range": "stddev: 0.000001782162922958587",
            "extra": "mean: 21.810376153358206 usec\nrounds: 32726"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 728.3773615236067,
            "unit": "iter/sec",
            "range": "stddev: 0.000032112513641313564",
            "extra": "mean: 1.3729147181458494 msec\nrounds: 777"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 625.9382909070433,
            "unit": "iter/sec",
            "range": "stddev: 0.00001555707286091842",
            "extra": "mean: 1.5976015759491342 msec\nrounds: 632"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1788119.6110395645,
            "unit": "iter/sec",
            "range": "stddev: 4.154205815132261e-8",
            "extra": "mean: 559.2467046533752 nsec\nrounds: 178222"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 47308.929284079815,
            "unit": "iter/sec",
            "range": "stddev: 0.0000014447833215650947",
            "extra": "mean: 21.13765868585226 usec\nrounds: 39219"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 887356.3399849435,
            "unit": "iter/sec",
            "range": "stddev: 8.022017721550388e-8",
            "extra": "mean: 1.1269429821360992 usec\nrounds: 92243"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 15350.625895573392,
            "unit": "iter/sec",
            "range": "stddev: 0.000002801383256978444",
            "extra": "mean: 65.1439235639484 usec\nrounds: 15333"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1250307.522390329,
            "unit": "iter/sec",
            "range": "stddev: 6.142806331189536e-8",
            "extra": "mean: 799.8032340780726 nsec\nrounds: 124147"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22617.724738702633,
            "unit": "iter/sec",
            "range": "stddev: 0.000002400876469707719",
            "extra": "mean: 44.21311213010017 usec\nrounds: 21582"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1170872.760545537,
            "unit": "iter/sec",
            "range": "stddev: 5.9145202548049107e-8",
            "extra": "mean: 854.063766531011 nsec\nrounds: 116199"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 18178.66110135915,
            "unit": "iter/sec",
            "range": "stddev: 0.0000029864498603593244",
            "extra": "mean: 55.009551826962316 usec\nrounds: 18089"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1615077.1080323136,
            "unit": "iter/sec",
            "range": "stddev: 4.578122725582994e-8",
            "extra": "mean: 619.165484438272 nsec\nrounds: 167197"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 51313.94772640283,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015477487411280018",
            "extra": "mean: 19.487878916115136 usec\nrounds: 49858"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 230164.75916728305,
            "unit": "iter/sec",
            "range": "stddev: 3.466900927072193e-7",
            "extra": "mean: 4.344713776417888 usec\nrounds: 114732"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2640.3184435378776,
            "unit": "iter/sec",
            "range": "stddev: 0.000006179013557647988",
            "extra": "mean: 378.7421939378102 usec\nrounds: 2640"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 241697.78789447024,
            "unit": "iter/sec",
            "range": "stddev: 3.99229238210042e-7",
            "extra": "mean: 4.137398230705441 usec\nrounds: 191939"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3412.5952293028968,
            "unit": "iter/sec",
            "range": "stddev: 0.000010009773704226186",
            "extra": "mean: 293.0321156793839 usec\nrounds: 3406"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 59183.363546605986,
            "unit": "iter/sec",
            "range": "stddev: 0.0000031972198403293912",
            "extra": "mean: 16.896640205528627 usec\nrounds: 50615"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 671.4147787663526,
            "unit": "iter/sec",
            "range": "stddev: 0.00002703513152894124",
            "extra": "mean: 1.4893922976157674 msec\nrounds: 672"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 135838.03247476925,
            "unit": "iter/sec",
            "range": "stddev: 5.705025604990345e-7",
            "extra": "mean: 7.36170851256802 usec\nrounds: 103435"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 160937.04653891726,
            "unit": "iter/sec",
            "range": "stddev: 5.661678534326906e-7",
            "extra": "mean: 6.213609740614839 usec\nrounds: 131510"
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
          "id": "8ace95e9cfab1bebcb2e3ed7617d2f1c093d3ed2",
          "message": "Improve relative time plot",
          "timestamp": "2024-03-21T08:57:19+01:00",
          "tree_id": "57282aa4b07e4c18bfb69a65895ccda11a94ee88",
          "url": "https://github.com/apytypes/apytypes/commit/8ace95e9cfab1bebcb2e3ed7617d2f1c093d3ed2"
        },
        "date": 1711008121011,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25025.5399707883,
            "unit": "iter/sec",
            "range": "stddev: 0.000001384860890545404",
            "extra": "mean: 39.9591777507009 usec\nrounds: 24405"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1777496.4263662861,
            "unit": "iter/sec",
            "range": "stddev: 6.663116343358517e-8",
            "extra": "mean: 562.5890354357866 nsec\nrounds: 172118"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 42105.05432863344,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018249070988351556",
            "extra": "mean: 23.750117793339417 usec\nrounds: 37795"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 724.3419340006794,
            "unit": "iter/sec",
            "range": "stddev: 0.000012573301917327905",
            "extra": "mean: 1.3805634508509097 msec\nrounds: 763"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 622.0260174821032,
            "unit": "iter/sec",
            "range": "stddev: 0.000018117176717437483",
            "extra": "mean: 1.607649795820272 msec\nrounds: 622"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1846012.75014637,
            "unit": "iter/sec",
            "range": "stddev: 4.0922129720282486e-8",
            "extra": "mean: 541.7080677913334 nsec\nrounds: 128288"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 44987.59774689105,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015728250566362277",
            "extra": "mean: 22.228348480089863 usec\nrounds: 35497"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 924094.034890979,
            "unit": "iter/sec",
            "range": "stddev: 6.625215528134249e-8",
            "extra": "mean: 1.08214095345608 usec\nrounds: 92422"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 15285.43813061497,
            "unit": "iter/sec",
            "range": "stddev: 0.000005612249882290447",
            "extra": "mean: 65.42174267135432 usec\nrounds: 15385"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1247085.4961620492,
            "unit": "iter/sec",
            "range": "stddev: 5.965458351345665e-8",
            "extra": "mean: 801.8696417185864 nsec\nrounds: 102575"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22573.892691746132,
            "unit": "iter/sec",
            "range": "stddev: 0.000002003694362564434",
            "extra": "mean: 44.29896135572744 usec\nrounds: 21866"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1123483.462419254,
            "unit": "iter/sec",
            "range": "stddev: 5.259178501431132e-8",
            "extra": "mean: 890.0887582685966 nsec\nrounds: 108969"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 18227.850761506637,
            "unit": "iter/sec",
            "range": "stddev: 0.000002798329265755804",
            "extra": "mean: 54.86110310447506 usec\nrounds: 17972"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1613966.0760015147,
            "unit": "iter/sec",
            "range": "stddev: 4.195210416426995e-8",
            "extra": "mean: 619.5917094351072 nsec\nrounds: 152161"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 52203.380455012346,
            "unit": "iter/sec",
            "range": "stddev: 0.0000015791048084727286",
            "extra": "mean: 19.155847596149002 usec\nrounds: 44264"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 227448.05572428385,
            "unit": "iter/sec",
            "range": "stddev: 2.8948141126352297e-7",
            "extra": "mean: 4.3966082577211205 usec\nrounds: 112791"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2642.378342401651,
            "unit": "iter/sec",
            "range": "stddev: 0.000012008419501433198",
            "extra": "mean: 378.44694075531305 usec\nrounds: 2650"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 237742.55701316046,
            "unit": "iter/sec",
            "range": "stddev: 3.580504003325676e-7",
            "extra": "mean: 4.2062305233162105 usec\nrounds: 194553"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3411.5174733273025,
            "unit": "iter/sec",
            "range": "stddev: 0.000010791409202247152",
            "extra": "mean: 293.12468947277165 usec\nrounds: 3420"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 59441.316113830384,
            "unit": "iter/sec",
            "range": "stddev: 0.000002058775735057285",
            "extra": "mean: 16.82331525239104 usec\nrounds: 53408"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 669.0098163815746,
            "unit": "iter/sec",
            "range": "stddev: 0.00005345034868958999",
            "extra": "mean: 1.4947463781751786 msec\nrounds: 669"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 139284.90755517344,
            "unit": "iter/sec",
            "range": "stddev: 5.759967191391857e-7",
            "extra": "mean: 7.179528762682926 usec\nrounds: 107101"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 161266.71779923915,
            "unit": "iter/sec",
            "range": "stddev: 5.397744389244906e-7",
            "extra": "mean: 6.200907500609639 usec\nrounds: 132909"
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
          "id": "081a78a10f101f7f997541da8235bf4ddd6aac6b",
          "message": "Improve relative time plot",
          "timestamp": "2024-03-21T10:00:07+01:00",
          "tree_id": "6656184cadd3dc10214fa488fce7605587fbd584",
          "url": "https://github.com/apytypes/apytypes/commit/081a78a10f101f7f997541da8235bf4ddd6aac6b"
        },
        "date": 1711011879769,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24891.02889486489,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016079913572819654",
            "extra": "mean: 40.17511707626934 usec\nrounds: 24215"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1792593.3779838448,
            "unit": "iter/sec",
            "range": "stddev: 8.497478412017072e-8",
            "extra": "mean: 557.8509952573389 nsec\nrounds: 54307"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 46836.7122008491,
            "unit": "iter/sec",
            "range": "stddev: 0.000001492673750567365",
            "extra": "mean: 21.350772780798888 usec\nrounds: 29751"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 748.8664848587869,
            "unit": "iter/sec",
            "range": "stddev: 0.00002148674882713137",
            "extra": "mean: 1.3353515215580374 msec\nrounds: 719"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 623.5600058917744,
            "unit": "iter/sec",
            "range": "stddev: 0.000020377850636348562",
            "extra": "mean: 1.6036948979270502 msec\nrounds: 627"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1759643.757656143,
            "unit": "iter/sec",
            "range": "stddev: 5.462335467927779e-8",
            "extra": "mean: 568.2968473868817 nsec\nrounds: 172682"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 46988.70197089521,
            "unit": "iter/sec",
            "range": "stddev: 0.0000014920757332287938",
            "extra": "mean: 21.281711519066857 usec\nrounds: 34394"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 889343.8960567212,
            "unit": "iter/sec",
            "range": "stddev: 7.612774466269448e-8",
            "extra": "mean: 1.1244244261797054 usec\nrounds: 91316"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 15389.576675797623,
            "unit": "iter/sec",
            "range": "stddev: 0.0000023926047424940436",
            "extra": "mean: 64.97904530230824 usec\nrounds: 15209"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1237617.350317675,
            "unit": "iter/sec",
            "range": "stddev: 6.851436983625787e-8",
            "extra": "mean: 808.0041862238805 nsec\nrounds: 124456"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 23033.20403132137,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021876102328901934",
            "extra": "mean: 43.41558380849509 usec\nrounds: 22098"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1205162.575507842,
            "unit": "iter/sec",
            "range": "stddev: 6.327857733551763e-8",
            "extra": "mean: 829.7635691008887 nsec\nrounds: 120686"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 18156.27630997598,
            "unit": "iter/sec",
            "range": "stddev: 0.000002976750570391386",
            "extra": "mean: 55.07737285593904 usec\nrounds: 17956"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1754268.578934218,
            "unit": "iter/sec",
            "range": "stddev: 4.323379038871859e-8",
            "extra": "mean: 570.0381412563098 nsec\nrounds: 173914"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 51905.73531793888,
            "unit": "iter/sec",
            "range": "stddev: 0.000001328377800762093",
            "extra": "mean: 19.26569374029068 usec\nrounds: 44619"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 228774.3579582509,
            "unit": "iter/sec",
            "range": "stddev: 2.6315222375621147e-7",
            "extra": "mean: 4.37111925009747 usec\nrounds: 114587"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 2644.3361698379904,
            "unit": "iter/sec",
            "range": "stddev: 0.0000056372350571485006",
            "extra": "mean: 378.16674423103575 usec\nrounds: 2600"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 250101.62908029536,
            "unit": "iter/sec",
            "range": "stddev: 2.942947986007288e-7",
            "extra": "mean: 3.9983745954687446 usec\nrounds: 118963"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3415.5568877525043,
            "unit": "iter/sec",
            "range": "stddev: 0.000005442439114443941",
            "extra": "mean: 292.7780250376733 usec\nrounds: 3355"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 58872.8066328324,
            "unit": "iter/sec",
            "range": "stddev: 0.000002191378645444687",
            "extra": "mean: 16.985770803091228 usec\nrounds: 42710"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 671.2654340187216,
            "unit": "iter/sec",
            "range": "stddev: 0.000012674177715108237",
            "extra": "mean: 1.4897236611949694 msec\nrounds: 670"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 136334.09780746518,
            "unit": "iter/sec",
            "range": "stddev: 4.953400652959118e-7",
            "extra": "mean: 7.334922195416056 usec\nrounds: 108027"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 158445.1080014156,
            "unit": "iter/sec",
            "range": "stddev: 5.401332192222392e-7",
            "extra": "mean: 6.311334017274081 usec\nrounds: 132556"
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
          "distinct": true,
          "id": "5fb5aee480e9440149e548ca399b3aa061218a04",
          "message": "src: fix APyFixed.__div__, closes #198",
          "timestamp": "2024-03-21T11:06:59+01:00",
          "tree_id": "ad81da3d6efed12f76dd5de6fed4d737efe06050",
          "url": "https://github.com/apytypes/apytypes/commit/5fb5aee480e9440149e548ca399b3aa061218a04"
        },
        "date": 1711015930751,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25312.11395648259,
            "unit": "iter/sec",
            "range": "stddev: 0.0000013344262701546965",
            "extra": "mean: 39.506775361363836 usec\nrounds: 24555"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1673195.8210451158,
            "unit": "iter/sec",
            "range": "stddev: 6.606714254126022e-8",
            "extra": "mean: 597.6586765411057 nsec\nrounds: 166639"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 38853.79992560689,
            "unit": "iter/sec",
            "range": "stddev: 0.00000211475639721074",
            "extra": "mean: 25.737508349625866 usec\nrounds: 31319"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 748.6687882838232,
            "unit": "iter/sec",
            "range": "stddev: 0.000030040226863742504",
            "extra": "mean: 1.3357041400006864 msec\nrounds: 800"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 626.4066839064387,
            "unit": "iter/sec",
            "range": "stddev: 0.00003638447375769889",
            "extra": "mean: 1.5964069759979793 msec\nrounds: 625"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1641297.9167192592,
            "unit": "iter/sec",
            "range": "stddev: 5.436191221025954e-8",
            "extra": "mean: 609.27391049081 nsec\nrounds: 162023"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 39785.837634471136,
            "unit": "iter/sec",
            "range": "stddev: 0.000002269316903553562",
            "extra": "mean: 25.134571984820617 usec\nrounds: 30826"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 962677.8459128585,
            "unit": "iter/sec",
            "range": "stddev: 6.329005233114503e-8",
            "extra": "mean: 1.0387691004271062 usec\nrounds: 98435"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17171.777334722006,
            "unit": "iter/sec",
            "range": "stddev: 0.0000032251310114643483",
            "extra": "mean: 58.23509008458669 usec\nrounds: 16762"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1276225.2426647241,
            "unit": "iter/sec",
            "range": "stddev: 7.821373280637503e-8",
            "extra": "mean: 783.5607434875884 nsec\nrounds: 120701"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22974.577966849694,
            "unit": "iter/sec",
            "range": "stddev: 0.0000033829555285518658",
            "extra": "mean: 43.52637081921211 usec\nrounds: 20568"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1304817.8369225888,
            "unit": "iter/sec",
            "range": "stddev: 6.952075688158502e-8",
            "extra": "mean: 766.3905042549412 nsec\nrounds: 130311"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 20331.322470116174,
            "unit": "iter/sec",
            "range": "stddev: 0.0000031600405937353957",
            "extra": "mean: 49.18519203410607 usec\nrounds: 19408"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1752144.274180421,
            "unit": "iter/sec",
            "range": "stddev: 4.535115009166948e-8",
            "extra": "mean: 570.7292571370668 nsec\nrounds: 152626"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 51099.19600792801,
            "unit": "iter/sec",
            "range": "stddev: 0.000001536844557159621",
            "extra": "mean: 19.569779529307088 usec\nrounds: 37175"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 1645242.7950962265,
            "unit": "iter/sec",
            "range": "stddev: 5.460094832422182e-8",
            "extra": "mean: 607.8130249107752 nsec\nrounds: 148523"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 43471.372829698346,
            "unit": "iter/sec",
            "range": "stddev: 0.000002231284022154463",
            "extra": "mean: 23.00364435044549 usec\nrounds: 39941"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 261579.72236373666,
            "unit": "iter/sec",
            "range": "stddev: 3.190615719907334e-7",
            "extra": "mean: 3.8229262993461766 usec\nrounds: 129300"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3726.71316978678,
            "unit": "iter/sec",
            "range": "stddev: 0.000007744169697837489",
            "extra": "mean: 268.33296646149296 usec\nrounds: 3727"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 81984.98572901233,
            "unit": "iter/sec",
            "range": "stddev: 9.89555916632627e-7",
            "extra": "mean: 12.19735529753378 usec\nrounds: 69750"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 943.8019784174107,
            "unit": "iter/sec",
            "range": "stddev: 0.000010428716248208112",
            "extra": "mean: 1.059544293048446 msec\nrounds: 935"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 168943.39237135425,
            "unit": "iter/sec",
            "range": "stddev: 6.442276506433263e-7",
            "extra": "mean: 5.919142417845507 usec\nrounds: 122625"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 198150.27367775328,
            "unit": "iter/sec",
            "range": "stddev: 5.036834694362299e-7",
            "extra": "mean: 5.046674836423765 usec\nrounds: 150309"
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
          "id": "6550e822faa3b418af34707dae8dae7edaab1107",
          "message": "Specialize conversion of numpy arrays to APyFloatArray",
          "timestamp": "2024-03-21T12:11:40+01:00",
          "tree_id": "af97207994a9f565b7b75c15f425f71655c29f1b",
          "url": "https://github.com/apytypes/apytypes/commit/6550e822faa3b418af34707dae8dae7edaab1107"
        },
        "date": 1711019693059,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24732.692672802314,
            "unit": "iter/sec",
            "range": "stddev: 0.000003413250552301789",
            "extra": "mean: 40.43231415314781 usec\nrounds: 24122"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1803834.065056105,
            "unit": "iter/sec",
            "range": "stddev: 5.013201132363142e-8",
            "extra": "mean: 554.3747173711952 nsec\nrounds: 177620"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 41126.073087832636,
            "unit": "iter/sec",
            "range": "stddev: 0.000002397709306529252",
            "extra": "mean: 24.31547495099539 usec\nrounds: 28624"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 714.6734317976151,
            "unit": "iter/sec",
            "range": "stddev: 0.000022126868110074966",
            "extra": "mean: 1.3992404859443344 msec\nrounds: 747"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 623.609294884893,
            "unit": "iter/sec",
            "range": "stddev: 0.000016643463221744497",
            "extra": "mean: 1.6035681446739534 msec\nrounds: 629"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1852965.3033889432,
            "unit": "iter/sec",
            "range": "stddev: 5.048930217245851e-8",
            "extra": "mean: 539.6755126344019 nsec\nrounds: 180832"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 44380.30858737686,
            "unit": "iter/sec",
            "range": "stddev: 0.0000022022863483944096",
            "extra": "mean: 22.532515699641422 usec\nrounds: 26434"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 940039.710667921,
            "unit": "iter/sec",
            "range": "stddev: 6.652607525897792e-8",
            "extra": "mean: 1.06378484722682 usec\nrounds: 93633"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 15438.357709275208,
            "unit": "iter/sec",
            "range": "stddev: 0.0000033104588695983344",
            "extra": "mean: 64.77372909938536 usec\nrounds: 15179"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1127565.522078825,
            "unit": "iter/sec",
            "range": "stddev: 5.9420723451676025e-8",
            "extra": "mean: 886.8664218788207 nsec\nrounds: 119962"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22514.025926896193,
            "unit": "iter/sec",
            "range": "stddev: 0.000003635328701165866",
            "extra": "mean: 44.416756169999715 usec\nrounds: 16450"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1283902.0125679963,
            "unit": "iter/sec",
            "range": "stddev: 6.835329655958049e-8",
            "extra": "mean: 778.8756386477545 nsec\nrounds: 118963"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 20408.134192760346,
            "unit": "iter/sec",
            "range": "stddev: 0.0000023549473199110567",
            "extra": "mean: 49.000069803281846 usec\nrounds: 17664"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1649038.5018711542,
            "unit": "iter/sec",
            "range": "stddev: 4.2490648674146614e-8",
            "extra": "mean: 606.4139793372779 nsec\nrounds: 150558"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 51852.51610737742,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016123278006603128",
            "extra": "mean: 19.285467226492468 usec\nrounds: 37881"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 1443712.3892922355,
            "unit": "iter/sec",
            "range": "stddev: 6.6208560938066e-8",
            "extra": "mean: 692.6587368902068 nsec\nrounds: 135981"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 31717.68045923785,
            "unit": "iter/sec",
            "range": "stddev: 0.000005816633841892808",
            "extra": "mean: 31.528156710108593 usec\nrounds: 27235"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 261836.31518083197,
            "unit": "iter/sec",
            "range": "stddev: 2.6043467056437833e-7",
            "extra": "mean: 3.8191799304438354 usec\nrounds: 114208"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3554.032728957936,
            "unit": "iter/sec",
            "range": "stddev: 0.000011106682051822835",
            "extra": "mean: 281.3705095769352 usec\nrounds: 3550"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 83994.56510548596,
            "unit": "iter/sec",
            "range": "stddev: 6.875244834243106e-7",
            "extra": "mean: 11.905532206091351 usec\nrounds: 73449"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 956.8379479125394,
            "unit": "iter/sec",
            "range": "stddev: 0.00007242687583030393",
            "extra": "mean: 1.045109051309706 msec\nrounds: 955"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 161552.94152807927,
            "unit": "iter/sec",
            "range": "stddev: 6.014707606806984e-7",
            "extra": "mean: 6.189921338115602 usec\nrounds: 125550"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 198867.14463121883,
            "unit": "iter/sec",
            "range": "stddev: 4.3678309764659455e-7",
            "extra": "mean: 5.028482718220798 usec\nrounds: 162049"
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
          "id": "f02a6dea59557a08d0a5fec7b472e2941500e18b",
          "message": "Speed-up array conversion for APyFloat",
          "timestamp": "2024-03-21T14:16:09+01:00",
          "tree_id": "ac6b39dffe2c2b3c5beadd22b11b7c1fc96fdde7",
          "url": "https://github.com/apytypes/apytypes/commit/f02a6dea59557a08d0a5fec7b472e2941500e18b"
        },
        "date": 1711027197750,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24905.899920221924,
            "unit": "iter/sec",
            "range": "stddev: 0.0000020234171589840555",
            "extra": "mean: 40.15112897759887 usec\nrounds: 24198"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1646881.481682167,
            "unit": "iter/sec",
            "range": "stddev: 5.65810858992642e-8",
            "extra": "mean: 607.2082363685266 nsec\nrounds: 163640"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 37219.64823141587,
            "unit": "iter/sec",
            "range": "stddev: 0.0000017937456163373215",
            "extra": "mean: 26.867529584976925 usec\nrounds: 33294"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 561.7290832869568,
            "unit": "iter/sec",
            "range": "stddev: 0.00003042947372191515",
            "extra": "mean: 1.7802175991111262 msec\nrounds: 676"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 627.958025243227,
            "unit": "iter/sec",
            "range": "stddev: 0.000015850819988398988",
            "extra": "mean: 1.5924631261980768 msec\nrounds: 626"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1680722.5277548367,
            "unit": "iter/sec",
            "range": "stddev: 4.8410665996395016e-8",
            "extra": "mean: 594.9822076436099 nsec\nrounds: 166639"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 38510.484621729025,
            "unit": "iter/sec",
            "range": "stddev: 0.000003395984935369716",
            "extra": "mean: 25.96695444948422 usec\nrounds: 35982"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 974032.0568478251,
            "unit": "iter/sec",
            "range": "stddev: 6.292947300598548e-8",
            "extra": "mean: 1.026660255142089 usec\nrounds: 95786"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17140.442512437705,
            "unit": "iter/sec",
            "range": "stddev: 0.0000022050546995593013",
            "extra": "mean: 58.34155094154454 usec\nrounds: 17098"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1288490.8370603686,
            "unit": "iter/sec",
            "range": "stddev: 5.461567094207042e-8",
            "extra": "mean: 776.1017550434567 nsec\nrounds: 120395"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22868.614648000585,
            "unit": "iter/sec",
            "range": "stddev: 0.0000034050509932562095",
            "extra": "mean: 43.72805329016423 usec\nrounds: 22537"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1302922.3226630492,
            "unit": "iter/sec",
            "range": "stddev: 5.6193613528024715e-8",
            "extra": "mean: 767.5054626096368 nsec\nrounds: 119247"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 20465.58053547069,
            "unit": "iter/sec",
            "range": "stddev: 0.0000020348935211991776",
            "extra": "mean: 48.86252790468428 usec\nrounds: 19943"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1760134.351392997,
            "unit": "iter/sec",
            "range": "stddev: 4.936220827682538e-8",
            "extra": "mean: 568.1384487545314 nsec\nrounds: 152161"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 52464.898735742565,
            "unit": "iter/sec",
            "range": "stddev: 0.0000013537701511407992",
            "extra": "mean: 19.0603627205466 usec\nrounds: 38010"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 1644769.334235863,
            "unit": "iter/sec",
            "range": "stddev: 4.344189339978045e-8",
            "extra": "mean: 607.9879890662437 nsec\nrounds: 148523"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 43348.545966742706,
            "unit": "iter/sec",
            "range": "stddev: 0.000001413074817303631",
            "extra": "mean: 23.0688245176022 usec\nrounds: 40118"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 263347.778049271,
            "unit": "iter/sec",
            "range": "stddev: 3.002964475524835e-7",
            "extra": "mean: 3.7972600619888484 usec\nrounds: 192308"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3722.5055753153756,
            "unit": "iter/sec",
            "range": "stddev: 0.000013466358611718128",
            "extra": "mean: 268.6362665595951 usec\nrounds: 3729"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 81296.39616007733,
            "unit": "iter/sec",
            "range": "stddev: 8.937701739139464e-7",
            "extra": "mean: 12.300668261246683 usec\nrounds: 72328"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 943.157129853358,
            "unit": "iter/sec",
            "range": "stddev: 0.000031487653166455715",
            "extra": "mean: 1.0602687170010368 msec\nrounds: 947"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 168061.73143202445,
            "unit": "iter/sec",
            "range": "stddev: 4.360924915699354e-7",
            "extra": "mean: 5.950194559339453 usec\nrounds: 121125"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 191881.5155268908,
            "unit": "iter/sec",
            "range": "stddev: 4.6398699908821994e-7",
            "extra": "mean: 5.211549415033973 usec\nrounds: 152393"
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
          "id": "bc76fdc0180387ded747b632f4283b7a7bf1e03d",
          "message": "Remove dead code",
          "timestamp": "2024-03-21T14:26:40+01:00",
          "tree_id": "49d0b8724c1964f2c690cc41a2a0cb7d29ca508b",
          "url": "https://github.com/apytypes/apytypes/commit/bc76fdc0180387ded747b632f4283b7a7bf1e03d"
        },
        "date": 1711027811701,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 24879.254399089332,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016825998810715767",
            "extra": "mean: 40.19413057798884 usec\nrounds: 24292"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1821226.579340396,
            "unit": "iter/sec",
            "range": "stddev: 6.433770112702228e-8",
            "extra": "mean: 549.0804995621176 nsec\nrounds: 181160"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 45172.7630193884,
            "unit": "iter/sec",
            "range": "stddev: 0.00000194863357525267",
            "extra": "mean: 22.137233438007645 usec\nrounds: 33405"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 562.8165621400069,
            "unit": "iter/sec",
            "range": "stddev: 0.00001572802595550946",
            "extra": "mean: 1.7767778478260894 msec\nrounds: 690"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 622.7192219815183,
            "unit": "iter/sec",
            "range": "stddev: 0.00001700749265881589",
            "extra": "mean: 1.60586017694774 msec\nrounds: 616"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1882175.4197519892,
            "unit": "iter/sec",
            "range": "stddev: 4.359175005624711e-8",
            "extra": "mean: 531.3001059868415 nsec\nrounds: 185874"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 46485.50692625514,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016650069895242098",
            "extra": "mean: 21.512081208158175 usec\nrounds: 32780"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 925788.5791070197,
            "unit": "iter/sec",
            "range": "stddev: 4.760229234068679e-8",
            "extra": "mean: 1.0801602250965054 usec\nrounds: 46381"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 15537.563517321232,
            "unit": "iter/sec",
            "range": "stddev: 0.000002484697011854003",
            "extra": "mean: 64.36015523831666 usec\nrounds: 15692"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1204344.6882721102,
            "unit": "iter/sec",
            "range": "stddev: 6.668246289623369e-8",
            "extra": "mean: 830.3270730863161 nsec\nrounds: 115661"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22746.561735496536,
            "unit": "iter/sec",
            "range": "stddev: 0.000003211489327729305",
            "extra": "mean: 43.962688147258625 usec\nrounds: 22315"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1303614.2787098833,
            "unit": "iter/sec",
            "range": "stddev: 7.02375755948912e-8",
            "extra": "mean: 767.0980721304367 nsec\nrounds: 128950"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 20642.28230433646,
            "unit": "iter/sec",
            "range": "stddev: 0.000003257757941840442",
            "extra": "mean: 48.44425559425294 usec\nrounds: 20513"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1801529.0250879226,
            "unit": "iter/sec",
            "range": "stddev: 4.1141563606281125e-8",
            "extra": "mean: 555.0840347693903 nsec\nrounds: 168322"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 51455.74316434876,
            "unit": "iter/sec",
            "range": "stddev: 0.0000014338988896280542",
            "extra": "mean: 19.434176605049064 usec\nrounds: 40214"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 1456591.6972209718,
            "unit": "iter/sec",
            "range": "stddev: 5.806613289459874e-8",
            "extra": "mean: 686.53418930485 nsec\nrounds: 139412"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 33487.482512523406,
            "unit": "iter/sec",
            "range": "stddev: 0.000002180345325943014",
            "extra": "mean: 29.86190435862198 usec\nrounds: 31890"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 263260.48342881934,
            "unit": "iter/sec",
            "range": "stddev: 3.44405687226405e-7",
            "extra": "mean: 3.7985191965598637 usec\nrounds: 185529"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3745.992892463305,
            "unit": "iter/sec",
            "range": "stddev: 0.0000036684797782550745",
            "extra": "mean: 266.9519213482586 usec\nrounds: 3738"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 81895.68667617696,
            "unit": "iter/sec",
            "range": "stddev: 0.0000022906427894817646",
            "extra": "mean: 12.210655293166923 usec\nrounds: 71043"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 957.6948403938519,
            "unit": "iter/sec",
            "range": "stddev: 0.00004223776061737564",
            "extra": "mean: 1.0441739454174674 msec\nrounds: 971"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 171405.23883098896,
            "unit": "iter/sec",
            "range": "stddev: 5.10996109540451e-7",
            "extra": "mean: 5.834127397856444 usec\nrounds: 128966"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 201315.73259121607,
            "unit": "iter/sec",
            "range": "stddev: 4.4694326578820295e-7",
            "extra": "mean: 4.967321664971715 usec\nrounds: 159949"
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
          "id": "1879554678b31d0089d3f976add183a1b029fa30",
          "message": "src: APyFixed::operator/() specialize for single limb",
          "timestamp": "2024-03-21T14:34:43+01:00",
          "tree_id": "51245ab8c4db8f2314cda6d31af54d00c25044e6",
          "url": "https://github.com/apytypes/apytypes/commit/1879554678b31d0089d3f976add183a1b029fa30"
        },
        "date": 1711028632335,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25217.727286660403,
            "unit": "iter/sec",
            "range": "stddev: 0.0000019360495880870746",
            "extra": "mean: 39.65464407765949 usec\nrounds: 23994"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1680317.089504176,
            "unit": "iter/sec",
            "range": "stddev: 7.159048007305207e-8",
            "extra": "mean: 595.1257689672141 nsec\nrounds: 170620"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 33418.973868698,
            "unit": "iter/sec",
            "range": "stddev: 0.00001878410582758784",
            "extra": "mean: 29.923121036838705 usec\nrounds: 24034"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 705.5822956633723,
            "unit": "iter/sec",
            "range": "stddev: 0.0000311790085867846",
            "extra": "mean: 1.4172691210453672 msec\nrounds: 727"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 606.7853402151615,
            "unit": "iter/sec",
            "range": "stddev: 0.00002121714251933886",
            "extra": "mean: 1.6480292678880604 msec\nrounds: 601"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1689681.2362731306,
            "unit": "iter/sec",
            "range": "stddev: 8.09989292648596e-8",
            "extra": "mean: 591.8276054278805 nsec\nrounds: 168322"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 39722.33728048512,
            "unit": "iter/sec",
            "range": "stddev: 0.0000022565962363015476",
            "extra": "mean: 25.174752254351418 usec\nrounds: 20404"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 960941.4914664975,
            "unit": "iter/sec",
            "range": "stddev: 7.021172740253192e-8",
            "extra": "mean: 1.0406460839503398 usec\nrounds: 97381"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17240.169606398526,
            "unit": "iter/sec",
            "range": "stddev: 0.000003317875414581501",
            "extra": "mean: 58.00406972961911 usec\nrounds: 14757"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1153450.6864555029,
            "unit": "iter/sec",
            "range": "stddev: 8.609405418816521e-8",
            "extra": "mean: 866.9638084597844 nsec\nrounds: 116320"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22489.70717402951,
            "unit": "iter/sec",
            "range": "stddev: 0.0000026208186064614507",
            "extra": "mean: 44.464785257620974 usec\nrounds: 17840"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1178650.7883555372,
            "unit": "iter/sec",
            "range": "stddev: 6.984637553579486e-8",
            "extra": "mean: 848.4277191170303 nsec\nrounds: 111396"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 18132.210469738173,
            "unit": "iter/sec",
            "range": "stddev: 0.0000031742279943734396",
            "extra": "mean: 55.15047388562768 usec\nrounds: 16217"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1478697.1165087174,
            "unit": "iter/sec",
            "range": "stddev: 5.7653337456917026e-8",
            "extra": "mean: 676.271015095383 nsec\nrounds: 154991"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 39786.19973285214,
            "unit": "iter/sec",
            "range": "stddev: 0.00000238129061717361",
            "extra": "mean: 25.134343232442053 usec\nrounds: 30542"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 1621162.637265623,
            "unit": "iter/sec",
            "range": "stddev: 6.348559914426232e-8",
            "extra": "mean: 616.8412576339042 nsec\nrounds: 154512"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 44236.22295132058,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018551707915082184",
            "extra": "mean: 22.605908309586972 usec\nrounds: 33984"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 268989.36714041256,
            "unit": "iter/sec",
            "range": "stddev: 3.4083746547901046e-7",
            "extra": "mean: 3.717619066622806 usec\nrounds: 126824"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3703.6645872235404,
            "unit": "iter/sec",
            "range": "stddev: 0.000007358983552277303",
            "extra": "mean: 270.0028516215212 usec\nrounds: 3700"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 85392.64115010588,
            "unit": "iter/sec",
            "range": "stddev: 0.0000010741031680261808",
            "extra": "mean: 11.710610967544245 usec\nrounds: 74711"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 992.353528921915,
            "unit": "iter/sec",
            "range": "stddev: 0.000012388768504201922",
            "extra": "mean: 1.007705390120789 msec\nrounds: 992"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 168398.96489559548,
            "unit": "iter/sec",
            "range": "stddev: 7.319752269655765e-7",
            "extra": "mean: 5.938278781107611 usec\nrounds: 119818"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 197970.77877196565,
            "unit": "iter/sec",
            "range": "stddev: 4.6358716413028327e-7",
            "extra": "mean: 5.0512505239566625 usec\nrounds: 150309"
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
          "distinct": true,
          "id": "16fa3b966d54c3882e6d9c770fbe9dad72203278",
          "message": "src: APyFixed.cc, explicit template instantiation",
          "timestamp": "2024-03-21T14:42:13+01:00",
          "tree_id": "7dbac5963ee33f6aa5c65d66c0cb57c87508bfe3",
          "url": "https://github.com/apytypes/apytypes/commit/16fa3b966d54c3882e6d9c770fbe9dad72203278"
        },
        "date": 1711028886543,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25011.115329350883,
            "unit": "iter/sec",
            "range": "stddev: 0.0000014885235603124814",
            "extra": "mean: 39.98222337675947 usec\nrounds: 24058"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1685476.500694167,
            "unit": "iter/sec",
            "range": "stddev: 5.8405688407469865e-8",
            "extra": "mean: 593.3040298027742 nsec\nrounds: 167477"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 40395.019657965735,
            "unit": "iter/sec",
            "range": "stddev: 0.0000018850551268036755",
            "extra": "mean: 24.7555270047456 usec\nrounds: 36901"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 579.4467031227916,
            "unit": "iter/sec",
            "range": "stddev: 0.000024395159827491017",
            "extra": "mean: 1.725784260417283 msec\nrounds: 672"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 619.0360784465985,
            "unit": "iter/sec",
            "range": "stddev: 0.000013490994737213312",
            "extra": "mean: 1.6154147307688231 msec\nrounds: 624"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1738627.2655556423,
            "unit": "iter/sec",
            "range": "stddev: 4.330654741978158e-8",
            "extra": "mean: 575.1664084712745 nsec\nrounds: 172088"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 40144.21211087385,
            "unit": "iter/sec",
            "range": "stddev: 0.0000019627725017347835",
            "extra": "mean: 24.91019121855253 usec\nrounds: 35598"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 979670.5327225364,
            "unit": "iter/sec",
            "range": "stddev: 6.717826765906262e-8",
            "extra": "mean: 1.0207513307774323 usec\nrounds: 97762"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17402.316385660077,
            "unit": "iter/sec",
            "range": "stddev: 0.0000024029481894337127",
            "extra": "mean: 57.46361448893228 usec\nrounds: 17089"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1180821.0168202748,
            "unit": "iter/sec",
            "range": "stddev: 5.051009131061337e-8",
            "extra": "mean: 846.8683955954702 nsec\nrounds: 118822"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22901.993774527662,
            "unit": "iter/sec",
            "range": "stddev: 0.0000020899451316827043",
            "extra": "mean: 43.6643206633054 usec\nrounds: 21652"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1167414.1255721014,
            "unit": "iter/sec",
            "range": "stddev: 5.1991510352038185e-8",
            "extra": "mean: 856.5940552671361 nsec\nrounds: 117028"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 18324.49426071533,
            "unit": "iter/sec",
            "range": "stddev: 0.000002291176764268426",
            "extra": "mean: 54.57176529798335 usec\nrounds: 17895"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1654710.1872298412,
            "unit": "iter/sec",
            "range": "stddev: 5.498120645820956e-8",
            "extra": "mean: 604.3354345175455 nsec\nrounds: 143617"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 40128.97942813281,
            "unit": "iter/sec",
            "range": "stddev: 0.000001629480193336405",
            "extra": "mean: 24.919646954662902 usec\nrounds: 31115"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 1688142.2684963346,
            "unit": "iter/sec",
            "range": "stddev: 4.7696467138416955e-8",
            "extra": "mean: 592.3671355556793 nsec\nrounds: 147016"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 43848.12399508728,
            "unit": "iter/sec",
            "range": "stddev: 0.0000013913411660017952",
            "extra": "mean: 22.805992797138583 usec\nrounds: 41094"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 269047.2275850246,
            "unit": "iter/sec",
            "range": "stddev: 2.840695087349774e-7",
            "extra": "mean: 3.7168195672411413 usec\nrounds: 118540"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3736.2448845311014,
            "unit": "iter/sec",
            "range": "stddev: 0.0000053420673463945005",
            "extra": "mean: 267.64840927323206 usec\nrounds: 3731"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 85441.72528398373,
            "unit": "iter/sec",
            "range": "stddev: 8.300605519406955e-7",
            "extra": "mean: 11.703883514480632 usec\nrounds: 73992"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 996.4017367094841,
            "unit": "iter/sec",
            "range": "stddev: 0.000007129407032688588",
            "extra": "mean: 1.0036112575459764 msec\nrounds: 994"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 173755.61323723075,
            "unit": "iter/sec",
            "range": "stddev: 4.61785389075767e-7",
            "extra": "mean: 5.75520975333722 usec\nrounds: 121271"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 199342.61624878627,
            "unit": "iter/sec",
            "range": "stddev: 5.071562402910052e-7",
            "extra": "mean: 5.016488791097065 usec\nrounds: 158178"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "committer": {
            "email": "40732757+Theodor-Lindberg@users.noreply.github.com",
            "name": "Theodor-Lindberg",
            "username": "Theodor-Lindberg"
          },
          "distinct": true,
          "id": "b14cc943a34395fed9f545287bb74b1ad87cf8a6",
          "message": "Only sanity check APyFloat format from Python",
          "timestamp": "2024-03-21T15:03:25+01:00",
          "tree_id": "3faae10f829b8b20a83733fc60d9f7ac2b67005d",
          "url": "https://github.com/apytypes/apytypes/commit/b14cc943a34395fed9f545287bb74b1ad87cf8a6"
        },
        "date": 1711029998657,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 25325.99968018144,
            "unit": "iter/sec",
            "range": "stddev: 0.0000022041675668337657",
            "extra": "mean: 39.48511461060067 usec\nrounds: 23052"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 1830373.6410080055,
            "unit": "iter/sec",
            "range": "stddev: 5.8177762937294006e-8",
            "extra": "mean: 546.3365389425735 nsec\nrounds: 180506"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 43019.6775854942,
            "unit": "iter/sec",
            "range": "stddev: 0.0000016556221184091794",
            "extra": "mean: 23.245176536078684 usec\nrounds: 31478"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_long_200",
            "value": 621.6565425568714,
            "unit": "iter/sec",
            "range": "stddev: 0.000023874799840767673",
            "extra": "mean: 1.6086052853027224 msec\nrounds: 694"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_different_wl_long_200",
            "value": 632.6470519029377,
            "unit": "iter/sec",
            "range": "stddev: 0.00003792859946428844",
            "extra": "mean: 1.5806601753570213 msec\nrounds: 633"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_20_different_wl",
            "value": 1850576.662647556,
            "unit": "iter/sec",
            "range": "stddev: 4.252991076721138e-8",
            "extra": "mean: 540.3721014018188 nsec\nrounds: 185529"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_addition_200_different_wl",
            "value": 45478.215637436784,
            "unit": "iter/sec",
            "range": "stddev: 0.0000017486078156556765",
            "extra": "mean: 21.98854959421098 usec\nrounds: 35357"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 1046524.1267752879,
            "unit": "iter/sec",
            "range": "stddev: 6.47046603683307e-8",
            "extra": "mean: 955.5441431449575 nsec\nrounds: 103221"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 17045.66089681363,
            "unit": "iter/sec",
            "range": "stddev: 0.0000027954758400151778",
            "extra": "mean: 58.665956459742276 usec\nrounds: 16835"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_20",
            "value": 1256086.727821398,
            "unit": "iter/sec",
            "range": "stddev: 8.387837275664829e-8",
            "extra": "mean: 796.123370982879 nsec\nrounds: 127470"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_negation_200",
            "value": 22926.552260387944,
            "unit": "iter/sec",
            "range": "stddev: 0.0000023898377909743586",
            "extra": "mean: 43.61754827514038 usec\nrounds: 22496"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_20",
            "value": 1273663.9273300155,
            "unit": "iter/sec",
            "range": "stddev: 6.491526165458827e-8",
            "extra": "mean: 785.1364701018531 nsec\nrounds: 112791"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_abs_200",
            "value": 20081.670736050663,
            "unit": "iter/sec",
            "range": "stddev: 0.000008006794808981908",
            "extra": "mean: 49.79665353265641 usec\nrounds: 19999"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 1676164.4653939288,
            "unit": "iter/sec",
            "range": "stddev: 5.204608882673567e-8",
            "extra": "mean: 596.600167015831 nsec\nrounds: 147886"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 52888.75964040225,
            "unit": "iter/sec",
            "range": "stddev: 0.0000014122771766449286",
            "extra": "mean: 18.907609231132167 usec\nrounds: 45001"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 1615858.9650720512,
            "unit": "iter/sec",
            "range": "stddev: 9.625833657275967e-8",
            "extra": "mean: 618.865892145095 nsec\nrounds: 146371"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 43142.15540425949,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021994934463944794",
            "extra": "mean: 23.179184967222767 usec\nrounds: 41110"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 261920.2441185735,
            "unit": "iter/sec",
            "range": "stddev: 3.844520795488719e-7",
            "extra": "mean: 3.8179561238775097 usec\nrounds: 191608"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 3721.5502197966757,
            "unit": "iter/sec",
            "range": "stddev: 0.000010075848125269402",
            "extra": "mean: 268.7052279129621 usec\nrounds: 3690"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_20",
            "value": 82466.4545954092,
            "unit": "iter/sec",
            "range": "stddev: 8.11163744142277e-7",
            "extra": "mean: 12.126142743811721 usec\nrounds: 71912"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_matrix_to_numpy_200",
            "value": 940.1944360502723,
            "unit": "iter/sec",
            "range": "stddev: 0.00004080409485638936",
            "extra": "mean: 1.0636097828880682 msec\nrounds: 935"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_sum_20",
            "value": 174516.96875352916,
            "unit": "iter/sec",
            "range": "stddev: 4.746850681783172e-7",
            "extra": "mean: 5.730101818421469 usec\nrounds: 128140"
          },
          {
            "name": "benchmark/ci_fixed_array_benchmarks.py::test_fixed_vector_max_20",
            "value": 197683.7750618715,
            "unit": "iter/sec",
            "range": "stddev: 5.848269500854139e-7",
            "extra": "mean: 5.058584093140763 usec\nrounds: 155958"
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
          "id": "1085f94a22bc3a38032b3bf5229073b8456b1983",
          "message": "More fixed-point array benchmarks",
          "timestamp": "2024-03-16T15:46:49+01:00",
          "tree_id": "eab1295361478d03a914832f57e36855cbc57364",
          "url": "https://github.com/apytypes/apytypes/commit/1085f94a22bc3a38032b3bf5229073b8456b1983"
        },
        "date": 1710600623483,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 201.5417333987829,
            "unit": "iter/sec",
            "range": "stddev: 0.000018353257226732277",
            "extra": "mean: 4.961751509903601 msec\nrounds: 202"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5697.068656631808,
            "unit": "iter/sec",
            "range": "stddev: 0.000004746487990456873",
            "extra": "mean: 175.52886585558807 usec\nrounds: 5673"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 57.02478028936478,
            "unit": "iter/sec",
            "range": "stddev: 0.00007553197954434798",
            "extra": "mean: 17.536235912275874 msec\nrounds: 57"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11691.640826570536,
            "unit": "iter/sec",
            "range": "stddev: 0.0000028023363877718797",
            "extra": "mean: 85.5311940243144 usec\nrounds: 11581"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 116.95575492828974,
            "unit": "iter/sec",
            "range": "stddev: 0.000025543495395609374",
            "extra": "mean: 8.55024193220026 msec\nrounds: 118"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6674.054012085636,
            "unit": "iter/sec",
            "range": "stddev: 0.000004358715872873414",
            "extra": "mean: 149.83396870764923 usec\nrounds: 6647"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 66.99590153854054,
            "unit": "iter/sec",
            "range": "stddev: 0.000025758381403390664",
            "extra": "mean: 14.926286191174439 msec\nrounds: 68"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12954.982687212645,
            "unit": "iter/sec",
            "range": "stddev: 0.000004281389164806978",
            "extra": "mean: 77.19037718105643 usec\nrounds: 12840"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 127.27092608179984,
            "unit": "iter/sec",
            "range": "stddev: 0.000023550830574883037",
            "extra": "mean: 7.857254054687068 msec\nrounds: 128"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9640.173750189908,
            "unit": "iter/sec",
            "range": "stddev: 0.000004198799251540649",
            "extra": "mean: 103.73257017077108 usec\nrounds: 9427"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 139.41489339811375,
            "unit": "iter/sec",
            "range": "stddev: 0.00009672944136684445",
            "extra": "mean: 7.172834807142132 msec\nrounds: 140"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73583.61083857597,
            "unit": "iter/sec",
            "range": "stddev: 6.536061609724635e-7",
            "extra": "mean: 13.589982723105962 usec\nrounds: 64479"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 838.1054096372252,
            "unit": "iter/sec",
            "range": "stddev: 0.000007406345587987728",
            "extra": "mean: 1.1931673373076677 msec\nrounds: 839"
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
          "id": "3cdd779f89b4253f80f05a35d5f25be8fb8e9dca",
          "message": "Compute shift amount less often",
          "timestamp": "2024-03-16T18:19:28+01:00",
          "tree_id": "181c28f8026c39ffd5cf445e4725d749df290e55",
          "url": "https://github.com/apytypes/apytypes/commit/3cdd779f89b4253f80f05a35d5f25be8fb8e9dca"
        },
        "date": 1710609779282,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 192.5372297244546,
            "unit": "iter/sec",
            "range": "stddev: 0.0001770366648258743",
            "extra": "mean: 5.193800707692366 msec\nrounds: 195"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5518.479243814573,
            "unit": "iter/sec",
            "range": "stddev: 0.000007055565879840861",
            "extra": "mean: 181.2093433387209 usec\nrounds: 4864"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 55.17258996489155,
            "unit": "iter/sec",
            "range": "stddev: 0.00044677237978083423",
            "extra": "mean: 18.124942124999727 msec\nrounds: 56"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11072.840260084493,
            "unit": "iter/sec",
            "range": "stddev: 0.000004797298196289291",
            "extra": "mean: 90.31106531942052 usec\nrounds: 7624"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 111.6801902618675,
            "unit": "iter/sec",
            "range": "stddev: 0.000268944277118696",
            "extra": "mean: 8.954139473215454 msec\nrounds: 112"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6456.254779775119,
            "unit": "iter/sec",
            "range": "stddev: 0.000006330194780920368",
            "extra": "mean: 154.88855909661476 usec\nrounds: 5271"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 66.3546795836777,
            "unit": "iter/sec",
            "range": "stddev: 0.0002979958188529354",
            "extra": "mean: 15.07052714705574 msec\nrounds: 68"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 11866.840373104715,
            "unit": "iter/sec",
            "range": "stddev: 0.000003983383696692824",
            "extra": "mean: 84.26842938465941 usec\nrounds: 10706"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 120.77684292207047,
            "unit": "iter/sec",
            "range": "stddev: 0.0003273362981850102",
            "extra": "mean: 8.279732900827982 msec\nrounds: 121"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9162.230345472919,
            "unit": "iter/sec",
            "range": "stddev: 0.000003387472915987878",
            "extra": "mean: 109.14373054308795 usec\nrounds: 9367"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 137.67840970676306,
            "unit": "iter/sec",
            "range": "stddev: 0.00016569981254228598",
            "extra": "mean: 7.263302954543626 msec\nrounds: 132"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73284.22114730986,
            "unit": "iter/sec",
            "range": "stddev: 6.852458159138702e-7",
            "extra": "mean: 13.645502187843178 usec\nrounds: 65364"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 832.3674352630142,
            "unit": "iter/sec",
            "range": "stddev: 0.000016871015710137928",
            "extra": "mean: 1.2013925072453329 msec\nrounds: 828"
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
          "id": "a78d22c8c9593f9c803cb11afde930b4e65a5d70",
          "message": "Inline scalar cast in array",
          "timestamp": "2024-03-16T19:17:31+01:00",
          "tree_id": "eeb97c983d61e9e692a2ea000ecf242953c9313e",
          "url": "https://github.com/apytypes/apytypes/commit/a78d22c8c9593f9c803cb11afde930b4e65a5d70"
        },
        "date": 1710613304159,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 197.5685586385173,
            "unit": "iter/sec",
            "range": "stddev: 0.00004549798995220395",
            "extra": "mean: 5.061534117023434 msec\nrounds: 188"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5773.269528514468,
            "unit": "iter/sec",
            "range": "stddev: 0.000004822298936428445",
            "extra": "mean: 173.21207594084248 usec\nrounds: 5715"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 56.71247066605492,
            "unit": "iter/sec",
            "range": "stddev: 0.00017959228315407472",
            "extra": "mean: 17.63280612280831 msec\nrounds: 57"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11210.956108165235,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021402650497351562",
            "extra": "mean: 89.19845821817762 usec\nrounds: 11237"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 113.27525483804676,
            "unit": "iter/sec",
            "range": "stddev: 0.000033263763111596425",
            "extra": "mean: 8.828053412280836 msec\nrounds: 114"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6707.92551168506,
            "unit": "iter/sec",
            "range": "stddev: 0.000005458208938251513",
            "extra": "mean: 149.07738588599736 usec\nrounds: 6717"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 68.70995774732074,
            "unit": "iter/sec",
            "range": "stddev: 0.00006338623237831549",
            "extra": "mean: 14.553931231881652 msec\nrounds: 69"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12632.429986951685,
            "unit": "iter/sec",
            "range": "stddev: 0.000003530521030110877",
            "extra": "mean: 79.16133325361169 usec\nrounds: 12579"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 120.36921108107923,
            "unit": "iter/sec",
            "range": "stddev: 0.00002827948839473564",
            "extra": "mean: 8.307772320003096 msec\nrounds: 125"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9606.12344010995,
            "unit": "iter/sec",
            "range": "stddev: 0.00000320422516433969",
            "extra": "mean: 104.10026544365894 usec\nrounds: 9486"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 140.01005188167377,
            "unit": "iter/sec",
            "range": "stddev: 0.00006046466499686036",
            "extra": "mean: 7.142344328570971 msec\nrounds: 140"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 74000.77405026057,
            "unit": "iter/sec",
            "range": "stddev: 6.563092798353994e-7",
            "extra": "mean: 13.513372161766986 usec\nrounds: 66721"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 838.714965633849,
            "unit": "iter/sec",
            "range": "stddev: 0.000006462959881126908",
            "extra": "mean: 1.1923001746418842 msec\nrounds: 836"
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
          "id": "0e8326562c018e28bef8d0facf1f1b0b82128c20",
          "message": "Remove unused arguments",
          "timestamp": "2024-03-16T19:30:41+01:00",
          "tree_id": "3eb7bbd376c47582af0b56edcd3bd0a27378db8e",
          "url": "https://github.com/apytypes/apytypes/commit/0e8326562c018e28bef8d0facf1f1b0b82128c20"
        },
        "date": 1710614036256,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 196.42553486286292,
            "unit": "iter/sec",
            "range": "stddev: 0.000028914146234870355",
            "extra": "mean: 5.090987791878297 msec\nrounds: 197"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5556.431749884922,
            "unit": "iter/sec",
            "range": "stddev: 0.0000033151586590352536",
            "extra": "mean: 179.97161578034513 usec\nrounds: 5640"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 55.46158784136436,
            "unit": "iter/sec",
            "range": "stddev: 0.00006423171672913407",
            "extra": "mean: 18.030497122806498 msec\nrounds: 57"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 10766.755871994239,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021583134533416102",
            "extra": "mean: 92.87848743753285 usec\nrounds: 10826"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 108.472850546641,
            "unit": "iter/sec",
            "range": "stddev: 0.00003224991268678226",
            "extra": "mean: 9.21889666364047 msec\nrounds: 110"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6843.857368757607,
            "unit": "iter/sec",
            "range": "stddev: 0.000003024017138320435",
            "extra": "mean: 146.11642910108367 usec\nrounds: 6749"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 66.7047900712868,
            "unit": "iter/sec",
            "range": "stddev: 0.00011284245142363544",
            "extra": "mean: 14.991427136361711 msec\nrounds: 66"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12288.778340322278,
            "unit": "iter/sec",
            "range": "stddev: 0.000003397958881003596",
            "extra": "mean: 81.37505391555257 usec\nrounds: 11926"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 124.16172442631131,
            "unit": "iter/sec",
            "range": "stddev: 0.0001250707649868532",
            "extra": "mean: 8.054011851240755 msec\nrounds: 121"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9635.959405411419,
            "unit": "iter/sec",
            "range": "stddev: 0.000002089674600446383",
            "extra": "mean: 103.7779382339878 usec\nrounds: 9536"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 138.9285060699592,
            "unit": "iter/sec",
            "range": "stddev: 0.00009856476904358772",
            "extra": "mean: 7.197946830986849 msec\nrounds: 142"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73823.32760345998,
            "unit": "iter/sec",
            "range": "stddev: 6.28069851571662e-7",
            "extra": "mean: 13.545853762803446 usec\nrounds: 66016"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 838.6666490691777,
            "unit": "iter/sec",
            "range": "stddev: 0.00000788195374962561",
            "extra": "mean: 1.1923688644467783 msec\nrounds: 841"
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
          "id": "7bbc14782cc3e9012e2c2a2fc782850da70caed2",
          "message": "Add test",
          "timestamp": "2024-03-16T20:08:08+01:00",
          "tree_id": "9e2df6f83d929296daf4e20e9b411e0019de64cd",
          "url": "https://github.com/apytypes/apytypes/commit/7bbc14782cc3e9012e2c2a2fc782850da70caed2"
        },
        "date": 1710616287592,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 197.22801090445898,
            "unit": "iter/sec",
            "range": "stddev: 0.00004464012354888872",
            "extra": "mean: 5.070273717278521 msec\nrounds: 191"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5609.259955773253,
            "unit": "iter/sec",
            "range": "stddev: 0.0000051005074340329405",
            "extra": "mean: 178.27663682635423 usec\nrounds: 5507"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 56.85427334390029,
            "unit": "iter/sec",
            "range": "stddev: 0.00007600348520820666",
            "extra": "mean: 17.58882738595914 msec\nrounds: 57"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11099.590189663497,
            "unit": "iter/sec",
            "range": "stddev: 0.0000025124104407243263",
            "extra": "mean: 90.09341632551902 usec\nrounds: 11234"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 113.50900451436415,
            "unit": "iter/sec",
            "range": "stddev: 0.00005698440795401447",
            "extra": "mean: 8.8098737565217 msec\nrounds: 115"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 6508.240507392075,
            "unit": "iter/sec",
            "range": "stddev: 0.0000034393302369704434",
            "extra": "mean: 153.65135920594784 usec\nrounds: 6751"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 66.5024439017428,
            "unit": "iter/sec",
            "range": "stddev: 0.0006072247444979422",
            "extra": "mean: 15.03704136764504 msec\nrounds: 68"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12480.057246268925,
            "unit": "iter/sec",
            "range": "stddev: 0.0000035684958889136298",
            "extra": "mean: 80.1278375785466 usec\nrounds: 12406"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 126.19937330351172,
            "unit": "iter/sec",
            "range": "stddev: 0.00003306299346817985",
            "extra": "mean: 7.923969619048602 msec\nrounds: 126"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9653.47071009143,
            "unit": "iter/sec",
            "range": "stddev: 0.000002630365337762243",
            "extra": "mean: 103.58968603433293 usec\nrounds: 9466"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 136.64431237101422,
            "unit": "iter/sec",
            "range": "stddev: 0.0001250019311351397",
            "extra": "mean: 7.318270205676894 msec\nrounds: 141"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 73894.7720660608,
            "unit": "iter/sec",
            "range": "stddev: 6.319591439263763e-7",
            "extra": "mean: 13.532757082003245 usec\nrounds: 65236"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 836.1215767529407,
            "unit": "iter/sec",
            "range": "stddev: 0.000011411260890612597",
            "extra": "mean: 1.1959983186697292 msec\nrounds: 841"
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
          "id": "cb733a73063b35d56a76fb9fcd535011749fcd08",
          "message": "Add early return for cast",
          "timestamp": "2024-03-16T21:27:41+01:00",
          "tree_id": "ae4553afcd999df438dddc2d3d0ff31514975ddb",
          "url": "https://github.com/apytypes/apytypes/commit/cb733a73063b35d56a76fb9fcd535011749fcd08"
        },
        "date": 1710621120370,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 205.68774144032776,
            "unit": "iter/sec",
            "range": "stddev: 0.000042893141765679034",
            "extra": "mean: 4.861738443902896 msec\nrounds: 205"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 6027.372280550337,
            "unit": "iter/sec",
            "range": "stddev: 0.000004291540816017429",
            "extra": "mean: 165.90977849947802 usec\nrounds: 6000"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 58.407825261338935,
            "unit": "iter/sec",
            "range": "stddev: 0.003146230839392069",
            "extra": "mean: 17.120993557380675 msec\nrounds: 61"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11861.015638152134,
            "unit": "iter/sec",
            "range": "stddev: 0.0000028049326185043",
            "extra": "mean: 84.30981211957943 usec\nrounds: 11832"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 118.15617374278838,
            "unit": "iter/sec",
            "range": "stddev: 0.00002897729538164551",
            "extra": "mean: 8.463374941176399 msec\nrounds: 119"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 7601.68894219086,
            "unit": "iter/sec",
            "range": "stddev: 0.000003000130482654957",
            "extra": "mean: 131.5497131762133 usec\nrounds: 7597"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 75.5489514550333,
            "unit": "iter/sec",
            "range": "stddev: 0.000028160955669780048",
            "extra": "mean: 13.236451078943158 msec\nrounds: 76"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12897.891067756074,
            "unit": "iter/sec",
            "range": "stddev: 0.0000021953976509215514",
            "extra": "mean: 77.53205502719261 usec\nrounds: 12830"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 129.39288779996915,
            "unit": "iter/sec",
            "range": "stddev: 0.00024490135500299386",
            "extra": "mean: 7.728400045804051 msec\nrounds: 131"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9585.649268766223,
            "unit": "iter/sec",
            "range": "stddev: 0.0000028273568804088853",
            "extra": "mean: 104.32261518876861 usec\nrounds: 9428"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 141.8776246240892,
            "unit": "iter/sec",
            "range": "stddev: 0.00005715761572601059",
            "extra": "mean: 7.048327758866435 msec\nrounds: 141"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 79015.5771348537,
            "unit": "iter/sec",
            "range": "stddev: 6.323291670603333e-7",
            "extra": "mean: 12.655732404426127 usec\nrounds: 69123"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 901.9896125698215,
            "unit": "iter/sec",
            "range": "stddev: 0.00001658063932811173",
            "extra": "mean: 1.10866021743969 msec\nrounds: 906"
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
          "id": "6f11740433df726d742b3675ec23303d2ca5a661",
          "message": "Add early return for cast and no quantization casting for arrays",
          "timestamp": "2024-03-16T23:56:33+01:00",
          "tree_id": "4373ecb5b36dff857ebbb054130efae2a3c81e8a",
          "url": "https://github.com/apytypes/apytypes/commit/6f11740433df726d742b3675ec23303d2ca5a661"
        },
        "date": 1710630000939,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 207.02891282978874,
            "unit": "iter/sec",
            "range": "stddev: 0.00004137518401805835",
            "extra": "mean: 4.830243207730901 msec\nrounds: 207"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 6206.280513202266,
            "unit": "iter/sec",
            "range": "stddev: 0.0000031776852694192427",
            "extra": "mean: 161.12710308094472 usec\nrounds: 6199"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 61.52476621713275,
            "unit": "iter/sec",
            "range": "stddev: 0.00007272081821976517",
            "extra": "mean: 16.253617225798262 msec\nrounds: 62"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11588.844062010014,
            "unit": "iter/sec",
            "range": "stddev: 0.0000022207529842492184",
            "extra": "mean: 86.28988315393349 usec\nrounds: 11605"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 118.03490528218298,
            "unit": "iter/sec",
            "range": "stddev: 0.00006160480970664963",
            "extra": "mean: 8.472070169492033 msec\nrounds: 118"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 7196.2594791013435,
            "unit": "iter/sec",
            "range": "stddev: 0.0000036606257469454566",
            "extra": "mean: 138.9610815040925 usec\nrounds: 7153"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 72.61538578335934,
            "unit": "iter/sec",
            "range": "stddev: 0.0000525493404254749",
            "extra": "mean: 13.77118621917673 msec\nrounds: 73"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12774.96656492536,
            "unit": "iter/sec",
            "range": "stddev: 0.000003156784851021839",
            "extra": "mean: 78.27809136859707 usec\nrounds: 12871"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 128.09585971244152,
            "unit": "iter/sec",
            "range": "stddev: 0.00003256062876053041",
            "extra": "mean: 7.806653565890961 msec\nrounds: 129"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9732.214710847842,
            "unit": "iter/sec",
            "range": "stddev: 0.0000027505176755776735",
            "extra": "mean: 102.75153494973426 usec\nrounds: 9571"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 143.09211504266958,
            "unit": "iter/sec",
            "range": "stddev: 0.00008344810897154233",
            "extra": "mean: 6.988505269502819 msec\nrounds: 141"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78757.0833412501,
            "unit": "iter/sec",
            "range": "stddev: 6.338283644746775e-7",
            "extra": "mean: 12.697270614594183 usec\nrounds: 69320"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 902.9298006475387,
            "unit": "iter/sec",
            "range": "stddev: 0.0000073991725748499544",
            "extra": "mean: 1.107505809734984 msec\nrounds: 904"
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
          "id": "794f95feac30dbced9c758d57e8c8bf546703adf",
          "message": "src: add faster APyFixedArray add/sub",
          "timestamp": "2024-03-18T09:40:47+01:00",
          "tree_id": "14f6e7a9f1597c5186a5f0637ca03ad956fabcdd",
          "url": "https://github.com/apytypes/apytypes/commit/794f95feac30dbced9c758d57e8c8bf546703adf"
        },
        "date": 1710751755316,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 208.56544448778976,
            "unit": "iter/sec",
            "range": "stddev: 0.00004328172451861373",
            "extra": "mean: 4.794658110579501 msec\nrounds: 208"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 6092.033066307451,
            "unit": "iter/sec",
            "range": "stddev: 0.000002988211871199651",
            "extra": "mean: 164.14881355956388 usec\nrounds: 6018"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 61.09284885059335,
            "unit": "iter/sec",
            "range": "stddev: 0.0000705558762453911",
            "extra": "mean: 16.36852788524508 msec\nrounds: 61"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11771.719844893392,
            "unit": "iter/sec",
            "range": "stddev: 0.000004134669649771698",
            "extra": "mean: 84.94935431493496 usec\nrounds: 11772"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 118.43069727207974,
            "unit": "iter/sec",
            "range": "stddev: 0.00002393830740615225",
            "extra": "mean: 8.44375675423598 msec\nrounds: 118"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 7541.231313672654,
            "unit": "iter/sec",
            "range": "stddev: 0.000004784326355459841",
            "extra": "mean: 132.60433984924273 usec\nrounds: 7571"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 72.13337021421127,
            "unit": "iter/sec",
            "range": "stddev: 0.000025977567763322297",
            "extra": "mean: 13.863209178086985 msec\nrounds: 73"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 11064.349219441214,
            "unit": "iter/sec",
            "range": "stddev: 0.000004113331942713934",
            "extra": "mean: 90.38037214542142 usec\nrounds: 11079"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 130.251769103154,
            "unit": "iter/sec",
            "range": "stddev: 0.000029548679479290954",
            "extra": "mean: 7.677438908396256 msec\nrounds: 131"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9673.789788418095,
            "unit": "iter/sec",
            "range": "stddev: 0.0000037213708869339814",
            "extra": "mean: 103.37210357798409 usec\nrounds: 9558"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 140.37966310118924,
            "unit": "iter/sec",
            "range": "stddev: 0.00003773975526343613",
            "extra": "mean: 7.123538965036371 msec\nrounds: 143"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78823.1885272709,
            "unit": "iter/sec",
            "range": "stddev: 6.515032037370202e-7",
            "extra": "mean: 12.68662202943014 usec\nrounds: 69852"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 902.1853001067763,
            "unit": "iter/sec",
            "range": "stddev: 0.000006020057658811637",
            "extra": "mean: 1.1084197446817712 msec\nrounds: 893"
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
          "id": "d18aef6266a3990a9fcfd7bafd748d9aab5b9a81",
          "message": "Faster unary operations (and some binary)",
          "timestamp": "2024-03-18T11:03:44+01:00",
          "tree_id": "43144e49a14d27b7d4f4b682e571c77f3d7f4f91",
          "url": "https://github.com/apytypes/apytypes/commit/d18aef6266a3990a9fcfd7bafd748d9aab5b9a81"
        },
        "date": 1710756450094,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 200.01770890459994,
            "unit": "iter/sec",
            "range": "stddev: 0.00003475351042719094",
            "extra": "mean: 4.999557316582194 msec\nrounds: 199"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 5963.118360490663,
            "unit": "iter/sec",
            "range": "stddev: 0.000004891872348626494",
            "extra": "mean: 167.6974930810726 usec\nrounds: 5926"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 59.199998511317276,
            "unit": "iter/sec",
            "range": "stddev: 0.000197495035015115",
            "extra": "mean: 16.891892316666695 msec\nrounds: 60"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 11275.239435096535,
            "unit": "iter/sec",
            "range": "stddev: 0.0000031586735446649087",
            "extra": "mean: 88.68991259620539 usec\nrounds: 11178"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 112.99228698885933,
            "unit": "iter/sec",
            "range": "stddev: 0.00006189078060403128",
            "extra": "mean: 8.850161605266 msec\nrounds: 114"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 7213.342803457516,
            "unit": "iter/sec",
            "range": "stddev: 0.000003099685834242851",
            "extra": "mean: 138.63198065682914 usec\nrounds: 7186"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 70.67616654021408,
            "unit": "iter/sec",
            "range": "stddev: 0.00007538967629011242",
            "extra": "mean: 14.149041309859516 msec\nrounds: 71"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 12225.718717229567,
            "unit": "iter/sec",
            "range": "stddev: 0.00000295502108659572",
            "extra": "mean: 81.7947822233724 usec\nrounds: 12297"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 124.69613543467716,
            "unit": "iter/sec",
            "range": "stddev: 0.00018641920590059468",
            "extra": "mean: 8.019494722223016 msec\nrounds: 126"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9628.232640202354,
            "unit": "iter/sec",
            "range": "stddev: 0.000006456902102089728",
            "extra": "mean: 103.86122119905312 usec\nrounds: 9557"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 142.78177846936563,
            "unit": "iter/sec",
            "range": "stddev: 0.00009186216095489347",
            "extra": "mean: 7.0036948041976785 msec\nrounds: 143"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78170.64777405524,
            "unit": "iter/sec",
            "range": "stddev: 7.003833262365278e-7",
            "extra": "mean: 12.792525436023046 usec\nrounds: 67719"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 900.9914512921971,
            "unit": "iter/sec",
            "range": "stddev: 0.00000854724980621331",
            "extra": "mean: 1.1098884440754742 msec\nrounds: 903"
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
          "id": "8624f1a830a788c51c318cf00d5e3a9111d50647",
          "message": "Use inline shift for APyFixed",
          "timestamp": "2024-03-18T12:24:47+01:00",
          "tree_id": "ce60e8d72d0599048f0325a173b54ba40c955da0",
          "url": "https://github.com/apytypes/apytypes/commit/8624f1a830a788c51c318cf00d5e3a9111d50647"
        },
        "date": 1710761295261,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 246.65810544253765,
            "unit": "iter/sec",
            "range": "stddev: 0.000018755662060662738",
            "extra": "mean: 4.054194765689399 msec\nrounds: 239"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7577.915738445617,
            "unit": "iter/sec",
            "range": "stddev: 0.000004067759458782092",
            "extra": "mean: 131.9624068827559 usec\nrounds: 7555"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 75.86301354953764,
            "unit": "iter/sec",
            "range": "stddev: 0.0000320830591074232",
            "extra": "mean: 13.181654052630165 msec\nrounds: 76"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 12646.777142672881,
            "unit": "iter/sec",
            "range": "stddev: 0.0000026488700376237536",
            "extra": "mean: 79.07152855772163 usec\nrounds: 12606"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 127.73192534465171,
            "unit": "iter/sec",
            "range": "stddev: 0.00002203611831676158",
            "extra": "mean: 7.82889631782937 msec\nrounds: 129"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 9578.819643307748,
            "unit": "iter/sec",
            "range": "stddev: 0.000004347037068516143",
            "extra": "mean: 104.39699641893257 usec\nrounds: 9494"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 95.36020833666382,
            "unit": "iter/sec",
            "range": "stddev: 0.00003325368783253889",
            "extra": "mean: 10.486554270829155 msec\nrounds: 96"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 14001.465951279777,
            "unit": "iter/sec",
            "range": "stddev: 0.0000034957069214383487",
            "extra": "mean: 71.42109286839332 usec\nrounds: 13966"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 142.21903460630938,
            "unit": "iter/sec",
            "range": "stddev: 0.00011273902615071639",
            "extra": "mean: 7.0314075944067485 msec\nrounds: 143"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9721.048557838481,
            "unit": "iter/sec",
            "range": "stddev: 0.000002993217653732732",
            "extra": "mean: 102.86956124642118 usec\nrounds: 9470"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 141.83765348463083,
            "unit": "iter/sec",
            "range": "stddev: 0.00014091086498106069",
            "extra": "mean: 7.050314041668472 msec\nrounds: 144"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78726.59063525361,
            "unit": "iter/sec",
            "range": "stddev: 6.368684213719579e-7",
            "extra": "mean: 12.702188573528826 usec\nrounds: 69750"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 901.1509228038842,
            "unit": "iter/sec",
            "range": "stddev: 0.000009168896465314582",
            "extra": "mean: 1.1096920334815306 msec\nrounds: 896"
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
          "id": "1cfd03b3ce602906dafa7e688b2d03ef70f9f403",
          "message": "Reuse variables for floating-point array arithmetic",
          "timestamp": "2024-03-18T12:45:51+01:00",
          "tree_id": "a7bcef98986f1516c9269c25df3071bc76bb461b",
          "url": "https://github.com/apytypes/apytypes/commit/1cfd03b3ce602906dafa7e688b2d03ef70f9f403"
        },
        "date": 1710762552295,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 250.77724042121343,
            "unit": "iter/sec",
            "range": "stddev: 0.00001908252090307522",
            "extra": "mean: 3.987602696003705 msec\nrounds: 250"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 8103.547296052191,
            "unit": "iter/sec",
            "range": "stddev: 0.00000478604435034495",
            "extra": "mean: 123.40274739769464 usec\nrounds: 7973"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 79.64052489033602,
            "unit": "iter/sec",
            "range": "stddev: 0.00015807013993946093",
            "extra": "mean: 12.556421512502425 msec\nrounds: 80"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 13514.379370657125,
            "unit": "iter/sec",
            "range": "stddev: 0.0000031052728717141882",
            "extra": "mean: 73.99525887006203 usec\nrounds: 13416"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 134.86090642990914,
            "unit": "iter/sec",
            "range": "stddev: 0.00026711728053843405",
            "extra": "mean: 7.415047299268504 msec\nrounds: 137"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 9866.143426900886,
            "unit": "iter/sec",
            "range": "stddev: 0.000004176848512590918",
            "extra": "mean: 101.35672640572145 usec\nrounds: 9854"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 97.92034951655094,
            "unit": "iter/sec",
            "range": "stddev: 0.00002613637862404117",
            "extra": "mean: 10.212381848483654 msec\nrounds: 99"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 14199.637662284797,
            "unit": "iter/sec",
            "range": "stddev: 0.0000031714092934461314",
            "extra": "mean: 70.42433221067802 usec\nrounds: 14253"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 143.5521680530987,
            "unit": "iter/sec",
            "range": "stddev: 0.000018329641864090633",
            "extra": "mean: 6.966108652779862 msec\nrounds: 144"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9816.542489751097,
            "unit": "iter/sec",
            "range": "stddev: 0.000003892988443592971",
            "extra": "mean: 101.86886075663035 usec\nrounds: 9566"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 144.59592317824504,
            "unit": "iter/sec",
            "range": "stddev: 0.00013310807439163036",
            "extra": "mean: 6.915824305553129 msec\nrounds: 144"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78593.17653736904,
            "unit": "iter/sec",
            "range": "stddev: 6.436514931852241e-7",
            "extra": "mean: 12.723750891078 usec\nrounds: 70993"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 901.4666659223542,
            "unit": "iter/sec",
            "range": "stddev: 0.00000943224855337699",
            "extra": "mean: 1.109303358407412 msec\nrounds: 904"
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
          "id": "ce8522b7e8c05ebc181b8b92a8314f805a068c34",
          "message": "Add fast path for floating-point multiplication",
          "timestamp": "2024-03-18T13:53:20+01:00",
          "tree_id": "e43a5b86300e1757e76647e397115aae9b8e8b56",
          "url": "https://github.com/apytypes/apytypes/commit/ce8522b7e8c05ebc181b8b92a8314f805a068c34"
        },
        "date": 1710766612679,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 363.05842626663366,
            "unit": "iter/sec",
            "range": "stddev: 0.00006950937447189968",
            "extra": "mean: 2.7543776088138228 msec\nrounds: 363"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7834.505593991782,
            "unit": "iter/sec",
            "range": "stddev: 0.000004374539022529182",
            "extra": "mean: 127.64047303340898 usec\nrounds: 7843"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 78.33855662272889,
            "unit": "iter/sec",
            "range": "stddev: 0.00046315323038839716",
            "extra": "mean: 12.765106265818835 msec\nrounds: 79"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 78033.10502112294,
            "unit": "iter/sec",
            "range": "stddev: 6.470668691109401e-7",
            "extra": "mean: 12.815073803987524 usec\nrounds: 68885"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 713.437255460815,
            "unit": "iter/sec",
            "range": "stddev: 0.000024193431032302783",
            "extra": "mean: 1.40166495700325 msec\nrounds: 721"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 9492.410791548504,
            "unit": "iter/sec",
            "range": "stddev: 0.0000040673973918134385",
            "extra": "mean: 105.34731607805494 usec\nrounds: 9485"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 95.98077249835923,
            "unit": "iter/sec",
            "range": "stddev: 0.000029759710983170783",
            "extra": "mean: 10.418753402063885 msec\nrounds: 97"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 79619.95093599788,
            "unit": "iter/sec",
            "range": "stddev: 0.0000024860094452896415",
            "extra": "mean: 12.559666116898832 usec\nrounds: 75787"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 814.5530937211693,
            "unit": "iter/sec",
            "range": "stddev: 0.00002428703134279292",
            "extra": "mean: 1.2276670578115947 msec\nrounds: 813"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9859.525495093132,
            "unit": "iter/sec",
            "range": "stddev: 0.0000023269842541374474",
            "extra": "mean: 101.42475928457996 usec\nrounds: 9559"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 140.13599405114545,
            "unit": "iter/sec",
            "range": "stddev: 0.00005928354131694883",
            "extra": "mean: 7.135925404254312 msec\nrounds: 141"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78981.3380625529,
            "unit": "iter/sec",
            "range": "stddev: 6.317662443922415e-7",
            "extra": "mean: 12.661218770540506 usec\nrounds: 68597"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 902.9427244239577,
            "unit": "iter/sec",
            "range": "stddev: 0.000006124377820990131",
            "extra": "mean: 1.1074899580568205 msec\nrounds: 906"
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
          "id": "4c6ca2566921c2fc9b46840887dee88ec93197c4",
          "message": "Add unary negation and specialize array/scalar operations",
          "timestamp": "2024-03-18T17:30:20+01:00",
          "tree_id": "cb4c776d6cce7c51122365014a72eeb43deb5473",
          "url": "https://github.com/apytypes/apytypes/commit/4c6ca2566921c2fc9b46840887dee88ec93197c4"
        },
        "date": 1710779625003,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 371.3344691136779,
            "unit": "iter/sec",
            "range": "stddev: 0.0001258897145909261",
            "extra": "mean: 2.6929899677421716 msec\nrounds: 372"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7885.934951421545,
            "unit": "iter/sec",
            "range": "stddev: 0.000011226951735761348",
            "extra": "mean: 126.80804573714327 usec\nrounds: 4176"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 79.81787602453204,
            "unit": "iter/sec",
            "range": "stddev: 0.00015178524792377297",
            "extra": "mean: 12.528521802467528 msec\nrounds: 81"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 81882.89764536291,
            "unit": "iter/sec",
            "range": "stddev: 7.006320806372655e-7",
            "extra": "mean: 12.212562436798798 usec\nrounds: 70191"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 751.9396320365911,
            "unit": "iter/sec",
            "range": "stddev: 0.000009223494734234936",
            "extra": "mean: 1.3298939933403295 msec\nrounds: 751"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 9458.715383674176,
            "unit": "iter/sec",
            "range": "stddev: 0.0000030515583997109936",
            "extra": "mean: 105.72260179495501 usec\nrounds: 9470"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 90.77025519351251,
            "unit": "iter/sec",
            "range": "stddev: 0.002409544833947743",
            "extra": "mean: 11.016824816324538 msec\nrounds: 98"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 83606.18246198227,
            "unit": "iter/sec",
            "range": "stddev: 7.358288876128946e-7",
            "extra": "mean: 11.960837949450974 usec\nrounds: 77556"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 846.8077069650625,
            "unit": "iter/sec",
            "range": "stddev: 0.000008978326145366342",
            "extra": "mean: 1.1809056433649794 msec\nrounds: 844"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9783.595361595027,
            "unit": "iter/sec",
            "range": "stddev: 0.0000023891154477432925",
            "extra": "mean: 102.21191321193082 usec\nrounds: 9045"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 141.73107951613923,
            "unit": "iter/sec",
            "range": "stddev: 0.00009583360515633355",
            "extra": "mean: 7.055615489657847 msec\nrounds: 145"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 77843.7403906796,
            "unit": "iter/sec",
            "range": "stddev: 0.0000012801269207731262",
            "extra": "mean: 12.846248073142851 usec\nrounds: 70842"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 903.3220341561621,
            "unit": "iter/sec",
            "range": "stddev: 0.0000060249658546587815",
            "extra": "mean: 1.1070249171262048 msec\nrounds: 905"
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
          "id": "386af56e4182053c65559ed6fbe59e22eff8b257",
          "message": "Special casing for double to fixed-point conversion",
          "timestamp": "2024-03-18T17:48:54+01:00",
          "tree_id": "22585c10515b3eaa1b1e87d7ca88e3d502dda7f9",
          "url": "https://github.com/apytypes/apytypes/commit/386af56e4182053c65559ed6fbe59e22eff8b257"
        },
        "date": 1710780733014,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 371.34980400030634,
            "unit": "iter/sec",
            "range": "stddev: 0.000013701932986851088",
            "extra": "mean: 2.6928787607470372 msec\nrounds: 372"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 8131.570045032018,
            "unit": "iter/sec",
            "range": "stddev: 0.0000025314860480851904",
            "extra": "mean: 122.97748091230547 usec\nrounds: 8068"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 80.17953207501499,
            "unit": "iter/sec",
            "range": "stddev: 0.0000250136114582906",
            "extra": "mean: 12.472010924987842 msec\nrounds: 80"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 82230.83923979735,
            "unit": "iter/sec",
            "range": "stddev: 8.131769767951252e-7",
            "extra": "mean: 12.160887682100038 usec\nrounds: 71912"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 752.5801059017325,
            "unit": "iter/sec",
            "range": "stddev: 0.00001287606809698854",
            "extra": "mean: 1.3287622037282159 msec\nrounds: 751"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 9530.212551280154,
            "unit": "iter/sec",
            "range": "stddev: 0.000003900173392179482",
            "extra": "mean: 104.92945405143918 usec\nrounds: 9565"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 96.90971540549214,
            "unit": "iter/sec",
            "range": "stddev: 0.00004094519690683586",
            "extra": "mean: 10.318882846944438 msec\nrounds: 98"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 83005.67330346153,
            "unit": "iter/sec",
            "range": "stddev: 0.0000011010789278774055",
            "extra": "mean: 12.04736929660322 usec\nrounds: 77737"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 818.0722054848155,
            "unit": "iter/sec",
            "range": "stddev: 0.000028741248079443305",
            "extra": "mean: 1.2223859865858275 msec\nrounds: 820"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9841.88221303047,
            "unit": "iter/sec",
            "range": "stddev: 0.0000026024341534031233",
            "extra": "mean: 101.6065807692779 usec\nrounds: 9682"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 143.42488514267535,
            "unit": "iter/sec",
            "range": "stddev: 0.00008564830031275104",
            "extra": "mean: 6.972290749999388 msec\nrounds: 144"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78788.91173352503,
            "unit": "iter/sec",
            "range": "stddev: 5.933837670684741e-7",
            "extra": "mean: 12.692141292446555 usec\nrounds: 69119"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 903.3597040462988,
            "unit": "iter/sec",
            "range": "stddev: 0.000006336850421245351",
            "extra": "mean: 1.1069787544439198 msec\nrounds: 900"
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
          "id": "6f1613718018f34beef7cc2801b3f740c395b1f8",
          "message": "src: fix memory out-of-bounds access bug, closes #179",
          "timestamp": "2024-03-18T18:18:25+01:00",
          "tree_id": "df4d00301812fd2d2eb9f063f7b2e3de1146007a",
          "url": "https://github.com/apytypes/apytypes/commit/6f1613718018f34beef7cc2801b3f740c395b1f8"
        },
        "date": 1710782906823,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 343.134183187711,
            "unit": "iter/sec",
            "range": "stddev: 0.00004245697541207376",
            "extra": "mean: 2.914311802776442 msec\nrounds: 360"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7824.524142756198,
            "unit": "iter/sec",
            "range": "stddev: 0.0000030440262227822265",
            "extra": "mean: 127.80329918539289 usec\nrounds: 7731"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 78.54084893643946,
            "unit": "iter/sec",
            "range": "stddev: 0.00003834828768759978",
            "extra": "mean: 12.732228051281535 msec\nrounds: 78"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 78145.09245127387,
            "unit": "iter/sec",
            "range": "stddev: 6.820360743558089e-7",
            "extra": "mean: 12.796708899199706 usec\nrounds: 71501"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 732.3913281012165,
            "unit": "iter/sec",
            "range": "stddev: 0.0001769095031146042",
            "extra": "mean: 1.3653902792549177 msec\nrounds: 752"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 9519.583242150715,
            "unit": "iter/sec",
            "range": "stddev: 0.000003563624948097589",
            "extra": "mean: 105.04661544133678 usec\nrounds: 9494"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 92.16346841742882,
            "unit": "iter/sec",
            "range": "stddev: 0.0004223143658015253",
            "extra": "mean: 10.85028609677294 msec\nrounds: 93"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 82330.79112905219,
            "unit": "iter/sec",
            "range": "stddev: 6.156262197235189e-7",
            "extra": "mean: 12.146124023422976 usec\nrounds: 78469"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 801.6170921896883,
            "unit": "iter/sec",
            "range": "stddev: 0.000010273594799272344",
            "extra": "mean: 1.2474783905473013 msec\nrounds: 804"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9509.162375765067,
            "unit": "iter/sec",
            "range": "stddev: 0.000014548430368471953",
            "extra": "mean: 105.16173354538437 usec\nrounds: 9739"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 143.10631594005216,
            "unit": "iter/sec",
            "range": "stddev: 0.00008400434283885379",
            "extra": "mean: 6.987811777775793 msec\nrounds: 144"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78707.30023930983,
            "unit": "iter/sec",
            "range": "stddev: 5.757246798818352e-7",
            "extra": "mean: 12.70530175675568 usec\nrounds: 68933"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 902.9787335760033,
            "unit": "iter/sec",
            "range": "stddev: 0.00000846243488862462",
            "extra": "mean: 1.107445793368544 msec\nrounds: 905"
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
          "id": "12af9d17ddfa5ee0d8bd3076a5cbdcd2a2b13d31",
          "message": "Add optmized two's overflow for single limb values",
          "timestamp": "2024-03-19T10:12:29+01:00",
          "tree_id": "5ecc3fa640bf11351eec27c66bf59994e2f9e629",
          "url": "https://github.com/apytypes/apytypes/commit/12af9d17ddfa5ee0d8bd3076a5cbdcd2a2b13d31"
        },
        "date": 1710839753540,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 354.34192463244585,
            "unit": "iter/sec",
            "range": "stddev: 0.00005503954788891126",
            "extra": "mean: 2.822132890533026 msec\nrounds: 338"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7810.836419474497,
            "unit": "iter/sec",
            "range": "stddev: 0.00000973364234192672",
            "extra": "mean: 128.02726190843447 usec\nrounds: 7789"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 77.76641116440962,
            "unit": "iter/sec",
            "range": "stddev: 0.0001582270227831147",
            "extra": "mean: 12.85902210256113 msec\nrounds: 78"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 82270.86436899526,
            "unit": "iter/sec",
            "range": "stddev: 0.0000010550929717677046",
            "extra": "mean: 12.154971357963047 usec\nrounds: 69653"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 751.2050312774468,
            "unit": "iter/sec",
            "range": "stddev: 0.00002723890869779828",
            "extra": "mean: 1.331194492001032 msec\nrounds: 750"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 9357.588442787295,
            "unit": "iter/sec",
            "range": "stddev: 0.0000031461782654100278",
            "extra": "mean: 106.8651401067747 usec\nrounds: 9350"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 92.28870465095504,
            "unit": "iter/sec",
            "range": "stddev: 0.00004649032029771056",
            "extra": "mean: 10.835562204303313 msec\nrounds: 93"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 82630.16540036793,
            "unit": "iter/sec",
            "range": "stddev: 6.630578472385395e-7",
            "extra": "mean: 12.102117854353798 usec\nrounds: 76900"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 838.4630217200336,
            "unit": "iter/sec",
            "range": "stddev: 0.000010532388352084449",
            "extra": "mean: 1.1926584406174376 msec\nrounds: 842"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9699.185101032008,
            "unit": "iter/sec",
            "range": "stddev: 0.000002854446470364597",
            "extra": "mean: 103.1014450784735 usec\nrounds: 9641"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 141.42534953186728,
            "unit": "iter/sec",
            "range": "stddev: 0.0000853579367149668",
            "extra": "mean: 7.070868152775332 msec\nrounds: 144"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78596.20138052916,
            "unit": "iter/sec",
            "range": "stddev: 6.47203639333033e-7",
            "extra": "mean: 12.72326120645994 usec\nrounds: 70339"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 903.3556110773786,
            "unit": "iter/sec",
            "range": "stddev: 0.000005749067018941507",
            "extra": "mean: 1.106983769998793 msec\nrounds: 900"
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
          "id": "e0905d02079c6768e085064ac9a7b9292781ede5",
          "message": "Increase performance for fixed-point array negation",
          "timestamp": "2024-03-19T10:41:59+01:00",
          "tree_id": "537ea6009f257c3640ed92077a3a43073f449e4c",
          "url": "https://github.com/apytypes/apytypes/commit/e0905d02079c6768e085064ac9a7b9292781ede5"
        },
        "date": 1710841530537,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 360.1132334064851,
            "unit": "iter/sec",
            "range": "stddev: 0.000019702486266168386",
            "extra": "mean: 2.7769043379509184 msec\nrounds: 361"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7722.180801493568,
            "unit": "iter/sec",
            "range": "stddev: 0.000007649204420608161",
            "extra": "mean: 129.497097478809 usec\nrounds: 7735"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 78.06355419361618,
            "unit": "iter/sec",
            "range": "stddev: 0.00007674129050899151",
            "extra": "mean: 12.810075205130453 msec\nrounds: 78"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 82238.95131939481,
            "unit": "iter/sec",
            "range": "stddev: 7.304124322303507e-7",
            "extra": "mean: 12.159688127785806 usec\nrounds: 71603"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 749.2932593057828,
            "unit": "iter/sec",
            "range": "stddev: 0.000009257772029952996",
            "extra": "mean: 1.3345909463091874 msec\nrounds: 745"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 9129.651016325894,
            "unit": "iter/sec",
            "range": "stddev: 0.000002862859806558045",
            "extra": "mean: 109.53321197182373 usec\nrounds: 9138"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 89.40507911328365,
            "unit": "iter/sec",
            "range": "stddev: 0.0001417011974528735",
            "extra": "mean: 11.185046866665338 msec\nrounds: 90"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 83946.46333359889,
            "unit": "iter/sec",
            "range": "stddev: 6.932652228080014e-7",
            "extra": "mean: 11.912354139638401 usec\nrounds: 77797"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 826.4702185017759,
            "unit": "iter/sec",
            "range": "stddev: 0.000009055502823469382",
            "extra": "mean: 1.2099649541066326 msec\nrounds: 828"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9723.76361812769,
            "unit": "iter/sec",
            "range": "stddev: 0.000004210575855804538",
            "extra": "mean: 102.84083810261833 usec\nrounds: 9506"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 141.56382494487698,
            "unit": "iter/sec",
            "range": "stddev: 0.00005928068984260628",
            "extra": "mean: 7.063951545455814 msec\nrounds: 143"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78899.44468038426,
            "unit": "iter/sec",
            "range": "stddev: 7.037483163817178e-7",
            "extra": "mean: 12.674360434993238 usec\nrounds: 69799"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 903.1165098776885,
            "unit": "iter/sec",
            "range": "stddev: 0.000006756564956257394",
            "extra": "mean: 1.1072768453047468 msec\nrounds: 905"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "committer": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "distinct": false,
          "id": "8fdcf13ac465a6ef94850f84f0c80dc9839539e0",
          "message": "Fix exception in multiplication",
          "timestamp": "2024-03-20T09:05:57+01:00",
          "tree_id": "dab4b4d2366191e3f34569bb6e583ce6d6aafc79",
          "url": "https://github.com/apytypes/apytypes/commit/8fdcf13ac465a6ef94850f84f0c80dc9839539e0"
        },
        "date": 1710922532748,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 226.2418708020391,
            "unit": "iter/sec",
            "range": "stddev: 0.000036419736102625514",
            "extra": "mean: 4.420048315791186 msec\nrounds: 228"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 4940.194929542079,
            "unit": "iter/sec",
            "range": "stddev: 0.000008984566372665919",
            "extra": "mean: 202.4211623756095 usec\nrounds: 4816"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 49.3934352088404,
            "unit": "iter/sec",
            "range": "stddev: 0.00008311513553280967",
            "extra": "mean: 20.245605428573647 msec\nrounds: 49"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 49443.6689769487,
            "unit": "iter/sec",
            "range": "stddev: 8.285373664387816e-7",
            "extra": "mean: 20.225036302751185 usec\nrounds: 47104"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 498.65356456678813,
            "unit": "iter/sec",
            "range": "stddev: 0.000008336002353517718",
            "extra": "mean: 2.0054002840003022 msec\nrounds: 500"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 5463.354984444363,
            "unit": "iter/sec",
            "range": "stddev: 0.0000052390888918053005",
            "extra": "mean: 183.03771269618542 usec\nrounds: 5482"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 40.802267195467685,
            "unit": "iter/sec",
            "range": "stddev: 0.0002569511898378246",
            "extra": "mean: 24.508442023806946 msec\nrounds: 42"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 47955.65637943012,
            "unit": "iter/sec",
            "range": "stddev: 7.559935554163345e-7",
            "extra": "mean: 20.852597493148597 usec\nrounds: 46993"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 489.9678037255983,
            "unit": "iter/sec",
            "range": "stddev: 0.000010310244951733252",
            "extra": "mean: 2.0409504306124577 msec\nrounds: 490"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9069.187819584922,
            "unit": "iter/sec",
            "range": "stddev: 0.0000029927144122715748",
            "extra": "mean: 110.26345687102199 usec\nrounds: 8834"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 125.43045175516626,
            "unit": "iter/sec",
            "range": "stddev: 0.00008367202131289883",
            "extra": "mean: 7.972545629923649 msec\nrounds: 127"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 57499.60354586808,
            "unit": "iter/sec",
            "range": "stddev: 7.944396602370661e-7",
            "extra": "mean: 17.391424259165348 usec\nrounds: 52508"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 619.3494112238344,
            "unit": "iter/sec",
            "range": "stddev: 0.00002480376168263419",
            "extra": "mean: 1.614597482258036 msec\nrounds: 620"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "committer": {
            "email": "40732757+Theodor-Lindberg@users.noreply.github.com",
            "name": "Theodor-Lindberg",
            "username": "Theodor-Lindberg"
          },
          "distinct": true,
          "id": "945083a637e09d72f4d413cef4e83360f300d646",
          "message": "Handle long formats in APyFloat power function",
          "timestamp": "2024-03-20T10:56:32+01:00",
          "tree_id": "d363a7dae42731cbd84aeadab196f860388baf8f",
          "url": "https://github.com/apytypes/apytypes/commit/945083a637e09d72f4d413cef4e83360f300d646"
        },
        "date": 1710928794775,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 222.6251075559404,
            "unit": "iter/sec",
            "range": "stddev: 0.00002220701882906804",
            "extra": "mean: 4.491856336324167 msec\nrounds: 223"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 4730.646860916119,
            "unit": "iter/sec",
            "range": "stddev: 0.000010187611900047481",
            "extra": "mean: 211.38758174106104 usec\nrounds: 4710"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 47.09735632383709,
            "unit": "iter/sec",
            "range": "stddev: 0.000728002183449989",
            "extra": "mean: 21.232614270832784 msec\nrounds: 48"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 49533.19857798057,
            "unit": "iter/sec",
            "range": "stddev: 8.320854294563005e-7",
            "extra": "mean: 20.188480225553995 usec\nrounds: 48219"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 498.0776732041802,
            "unit": "iter/sec",
            "range": "stddev: 0.00000987921040294954",
            "extra": "mean: 2.0077189840028495 msec\nrounds: 500"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 5325.49090220579,
            "unit": "iter/sec",
            "range": "stddev: 0.0000038029840944076727",
            "extra": "mean: 187.77611648642667 usec\nrounds: 5271"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 53.5036617252278,
            "unit": "iter/sec",
            "range": "stddev: 0.0002756298378074344",
            "extra": "mean: 18.690309555551124 msec\nrounds: 54"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 48165.7490229299,
            "unit": "iter/sec",
            "range": "stddev: 8.017522311733967e-7",
            "extra": "mean: 20.76164121363373 usec\nrounds: 47329"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 487.26392832524726,
            "unit": "iter/sec",
            "range": "stddev: 0.000007677013375278888",
            "extra": "mean: 2.05227586502669 msec\nrounds: 489"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 8882.10824086916,
            "unit": "iter/sec",
            "range": "stddev: 0.0000027452334108309485",
            "extra": "mean: 112.58588309008773 usec\nrounds: 8776"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 125.06382830750925,
            "unit": "iter/sec",
            "range": "stddev: 0.00004778982971157749",
            "extra": "mean: 7.995917073169881 msec\nrounds: 123"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 57250.24279580358,
            "unit": "iter/sec",
            "range": "stddev: 7.928810343972672e-7",
            "extra": "mean: 17.467174830449796 usec\nrounds: 52508"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 618.1035470199979,
            "unit": "iter/sec",
            "range": "stddev: 0.00002063506420088329",
            "extra": "mean: 1.617851903327852 msec\nrounds: 631"
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
          "id": "c478d297ca4154ef10258fa8f0e5949310cc8476",
          "message": "Restructure code for readability",
          "timestamp": "2024-03-20T12:11:04+01:00",
          "tree_id": "ab5740e9d48836c176f83823695ab1ea4f7b0e73",
          "url": "https://github.com/apytypes/apytypes/commit/c478d297ca4154ef10258fa8f0e5949310cc8476"
        },
        "date": 1710933267839,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 228.37797126582691,
            "unit": "iter/sec",
            "range": "stddev: 0.00004068627692586299",
            "extra": "mean: 4.378706030434179 msec\nrounds: 230"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 4912.982080544236,
            "unit": "iter/sec",
            "range": "stddev: 0.00001706760631128207",
            "extra": "mean: 203.54236665345724 usec\nrounds: 4912"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 48.82447819797509,
            "unit": "iter/sec",
            "range": "stddev: 0.00019887658619465443",
            "extra": "mean: 20.48152969387952 msec\nrounds: 49"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 49296.22142380781,
            "unit": "iter/sec",
            "range": "stddev: 7.841121668431933e-7",
            "extra": "mean: 20.285530434530344 usec\nrounds: 47282"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 498.1011484038153,
            "unit": "iter/sec",
            "range": "stddev: 0.000009056499556617069",
            "extra": "mean: 2.007624361446544 msec\nrounds: 498"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 5532.41256675099,
            "unit": "iter/sec",
            "range": "stddev: 0.000007067773307244796",
            "extra": "mean: 180.75296951096115 usec\nrounds: 5543"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 54.58657590331851,
            "unit": "iter/sec",
            "range": "stddev: 0.00016906077286846187",
            "extra": "mean: 18.319522399997368 msec\nrounds: 55"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 47860.90723399947,
            "unit": "iter/sec",
            "range": "stddev: 7.701331559424246e-7",
            "extra": "mean: 20.89387890436016 usec\nrounds: 46905"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 484.87708127534046,
            "unit": "iter/sec",
            "range": "stddev: 0.00008064580623976645",
            "extra": "mean: 2.062378360655376 msec\nrounds: 488"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9200.83545917014,
            "unit": "iter/sec",
            "range": "stddev: 0.00000437199505094475",
            "extra": "mean: 108.68578233331365 usec\nrounds: 9000"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 131.67385859975934,
            "unit": "iter/sec",
            "range": "stddev: 0.00007115703752673519",
            "extra": "mean: 7.594521878785647 msec\nrounds: 132"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 57064.716677190234,
            "unit": "iter/sec",
            "range": "stddev: 6.96649384951759e-7",
            "extra": "mean: 17.52396328640176 usec\nrounds: 52869"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 629.7549419148412,
            "unit": "iter/sec",
            "range": "stddev: 0.000011178622976392939",
            "extra": "mean: 1.5879192578614578 msec\nrounds: 636"
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
          "id": "68cddb2248c5f525387429bbf6d8ab139744fdfe",
          "message": "Add test for long integers and APyFloat",
          "timestamp": "2024-03-20T12:41:56+01:00",
          "tree_id": "67d63d3b788f6fbeb6a96785d86e2fc9a357b73a",
          "url": "https://github.com/apytypes/apytypes/commit/68cddb2248c5f525387429bbf6d8ab139744fdfe"
        },
        "date": 1710935147633,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 219.522001367038,
            "unit": "iter/sec",
            "range": "stddev: 0.00013858053925486672",
            "extra": "mean: 4.555352054794785 msec\nrounds: 219"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 4730.338415494303,
            "unit": "iter/sec",
            "range": "stddev: 0.00000908017448921017",
            "extra": "mean: 211.4013654339155 usec\nrounds: 4704"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 47.322803908911126,
            "unit": "iter/sec",
            "range": "stddev: 0.000211217864672752",
            "extra": "mean: 21.131461312496214 msec\nrounds: 48"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 58111.34010216902,
            "unit": "iter/sec",
            "range": "stddev: 6.782824119360197e-7",
            "extra": "mean: 17.208345191176804 usec\nrounds: 52258"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 545.154522413985,
            "unit": "iter/sec",
            "range": "stddev: 0.000008244394558613347",
            "extra": "mean: 1.8343422990823322 msec\nrounds: 545"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 5374.084263513322,
            "unit": "iter/sec",
            "range": "stddev: 0.000003898633633483091",
            "extra": "mean: 186.07821369481974 usec\nrounds: 5316"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 54.839061563497665,
            "unit": "iter/sec",
            "range": "stddev: 0.0010789177428548755",
            "extra": "mean: 18.235177107144857 msec\nrounds: 56"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 62517.99113151213,
            "unit": "iter/sec",
            "range": "stddev: 6.664353262593325e-7",
            "extra": "mean: 15.995395595747974 usec\nrounds: 59806"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 652.390284211993,
            "unit": "iter/sec",
            "range": "stddev: 0.00005421657193044528",
            "extra": "mean: 1.5328247893940918 msec\nrounds: 660"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9140.32034253968,
            "unit": "iter/sec",
            "range": "stddev: 0.00000497554562153205",
            "extra": "mean: 109.40535588735672 usec\nrounds: 9104"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 130.47426153068545,
            "unit": "iter/sec",
            "range": "stddev: 0.000032558967712715485",
            "extra": "mean: 7.664346885495236 msec\nrounds: 131"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 58513.65929766248,
            "unit": "iter/sec",
            "range": "stddev: 8.185967277462502e-7",
            "extra": "mean: 17.090026704926114 usec\nrounds: 53548"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 622.010599752832,
            "unit": "iter/sec",
            "range": "stddev: 0.0002457668749009791",
            "extra": "mean: 1.6076896445130828 msec\nrounds: 647"
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
          "id": "0209a95a24ff85505f46fe4b2038518b930485d7",
          "message": "Refactor left-shift to speed up cast",
          "timestamp": "2024-03-20T19:59:06+01:00",
          "tree_id": "72307b75fd30328d7199e24cae5e9c1ee3752721",
          "url": "https://github.com/apytypes/apytypes/commit/0209a95a24ff85505f46fe4b2038518b930485d7"
        },
        "date": 1710961361638,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 231.48051166715356,
            "unit": "iter/sec",
            "range": "stddev: 0.00004290732773352339",
            "extra": "mean: 4.320018099138742 msec\nrounds: 232"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 4804.298334698388,
            "unit": "iter/sec",
            "range": "stddev: 0.000008717560814950971",
            "extra": "mean: 208.14694057977968 usec\nrounds: 4830"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 47.13818236933329,
            "unit": "iter/sec",
            "range": "stddev: 0.00008412448392911641",
            "extra": "mean: 21.214224854171942 msec\nrounds: 48"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 60672.47271922371,
            "unit": "iter/sec",
            "range": "stddev: 7.518067810358004e-7",
            "extra": "mean: 16.481939093330475 usec\nrounds: 55823"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 576.119532236587,
            "unit": "iter/sec",
            "range": "stddev: 0.000012560024273189541",
            "extra": "mean: 1.73575090592371 msec\nrounds: 574"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 5516.561904325603,
            "unit": "iter/sec",
            "range": "stddev: 0.0000036224688560440548",
            "extra": "mean: 181.2723245643066 usec\nrounds: 5512"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 56.29428299032693,
            "unit": "iter/sec",
            "range": "stddev: 0.000044822111284353076",
            "extra": "mean: 17.763793175442533 msec\nrounds: 57"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 57695.44341013963,
            "unit": "iter/sec",
            "range": "stddev: 8.860308616858325e-7",
            "extra": "mean: 17.332391275534526 usec\nrounds: 54813"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 587.3659630123985,
            "unit": "iter/sec",
            "range": "stddev: 0.000006881493952613441",
            "extra": "mean: 1.7025160853232677 msec\nrounds: 586"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9354.34978804866,
            "unit": "iter/sec",
            "range": "stddev: 0.000003968698909042523",
            "extra": "mean: 106.90213886139087 usec\nrounds: 9189"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 132.2615409474359,
            "unit": "iter/sec",
            "range": "stddev: 0.0000503270804919436",
            "extra": "mean: 7.560776873130682 msec\nrounds: 134"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 58458.000482919575,
            "unit": "iter/sec",
            "range": "stddev: 7.416712360897115e-7",
            "extra": "mean: 17.106298397807546 usec\nrounds: 52869"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 644.3520818227487,
            "unit": "iter/sec",
            "range": "stddev: 0.00000703622556002755",
            "extra": "mean: 1.5519465649450397 msec\nrounds: 639"
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
          "id": "8ace95e9cfab1bebcb2e3ed7617d2f1c093d3ed2",
          "message": "Improve relative time plot",
          "timestamp": "2024-03-21T08:57:19+01:00",
          "tree_id": "57282aa4b07e4c18bfb69a65895ccda11a94ee88",
          "url": "https://github.com/apytypes/apytypes/commit/8ace95e9cfab1bebcb2e3ed7617d2f1c093d3ed2"
        },
        "date": 1711008139233,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 212.73335362123487,
            "unit": "iter/sec",
            "range": "stddev: 0.000031595857977809396",
            "extra": "mean: 4.700720328888666 msec\nrounds: 225"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 4770.8048167187135,
            "unit": "iter/sec",
            "range": "stddev: 0.000006576152152036122",
            "extra": "mean: 209.60823978705224 usec\nrounds: 4700"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 48.14267814437366,
            "unit": "iter/sec",
            "range": "stddev: 0.0000962032829552484",
            "extra": "mean: 20.771590583330852 msec\nrounds: 48"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 61049.414454386206,
            "unit": "iter/sec",
            "range": "stddev: 7.24584323791831e-7",
            "extra": "mean: 16.380173486302017 usec\nrounds: 54304"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 575.7008560487682,
            "unit": "iter/sec",
            "range": "stddev: 0.000007396165306561825",
            "extra": "mean: 1.7370132239568685 msec\nrounds: 576"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 5632.435427483372,
            "unit": "iter/sec",
            "range": "stddev: 0.000006452502382810221",
            "extra": "mean: 177.5430917717258 usec\nrounds: 5481"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 54.888367357270496,
            "unit": "iter/sec",
            "range": "stddev: 0.000042138324690978526",
            "extra": "mean: 18.218796589283873 msec\nrounds: 56"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 58532.16483395576,
            "unit": "iter/sec",
            "range": "stddev: 6.631509981135465e-7",
            "extra": "mean: 17.084623520021914 usec\nrounds: 57007"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 597.2511841645478,
            "unit": "iter/sec",
            "range": "stddev: 0.000018985217674344558",
            "extra": "mean: 1.674337408637923 msec\nrounds: 602"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9375.651191611765,
            "unit": "iter/sec",
            "range": "stddev: 0.000002344892025188979",
            "extra": "mean: 106.65925806782178 usec\nrounds: 9234"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 132.20693531901892,
            "unit": "iter/sec",
            "range": "stddev: 0.0001118451788047712",
            "extra": "mean: 7.563899712121552 msec\nrounds: 132"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 58339.043960492236,
            "unit": "iter/sec",
            "range": "stddev: 7.12039193272308e-7",
            "extra": "mean: 17.14117908200912 usec\nrounds: 53093"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 644.4412102781832,
            "unit": "iter/sec",
            "range": "stddev: 0.000006087543387378345",
            "extra": "mean: 1.5517319253502337 msec\nrounds: 643"
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
          "id": "081a78a10f101f7f997541da8235bf4ddd6aac6b",
          "message": "Improve relative time plot",
          "timestamp": "2024-03-21T10:00:07+01:00",
          "tree_id": "6656184cadd3dc10214fa488fce7605587fbd584",
          "url": "https://github.com/apytypes/apytypes/commit/081a78a10f101f7f997541da8235bf4ddd6aac6b"
        },
        "date": 1711011898905,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 229.94599748956028,
            "unit": "iter/sec",
            "range": "stddev: 0.0001108500020424165",
            "extra": "mean: 4.348847168106941 msec\nrounds: 232"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 4760.305087054878,
            "unit": "iter/sec",
            "range": "stddev: 0.000004208758532616111",
            "extra": "mean: 210.07056936736873 usec\nrounds: 4505"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 47.81418985059452,
            "unit": "iter/sec",
            "range": "stddev: 0.00006971282363021533",
            "extra": "mean: 20.91429349999885 msec\nrounds: 48"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 60616.64701554978,
            "unit": "iter/sec",
            "range": "stddev: 7.238533135799044e-7",
            "extra": "mean: 16.497118353370375 usec\nrounds: 54219"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 574.6329338846388,
            "unit": "iter/sec",
            "range": "stddev: 0.000007958605534641805",
            "extra": "mean: 1.7402413628466975 msec\nrounds: 576"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 5588.029295890742,
            "unit": "iter/sec",
            "range": "stddev: 0.000005440490992203104",
            "extra": "mean: 178.95396517254625 usec\nrounds: 5599"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 54.897219449447014,
            "unit": "iter/sec",
            "range": "stddev: 0.000040696673854002415",
            "extra": "mean: 18.21585883636358 msec\nrounds: 55"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 58349.57333657913,
            "unit": "iter/sec",
            "range": "stddev: 7.104009691415852e-7",
            "extra": "mean: 17.138085898789317 usec\nrounds: 56648"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 598.1004806421765,
            "unit": "iter/sec",
            "range": "stddev: 0.000013860786786396477",
            "extra": "mean: 1.671959866887762 msec\nrounds: 601"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9281.655844470022,
            "unit": "iter/sec",
            "range": "stddev: 0.000004095831835996007",
            "extra": "mean: 107.73939658577154 usec\nrounds: 9138"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 131.90361085522144,
            "unit": "iter/sec",
            "range": "stddev: 0.00010087975719726067",
            "extra": "mean: 7.581293593983631 msec\nrounds: 133"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 58384.698540401645,
            "unit": "iter/sec",
            "range": "stddev: 7.635015462466176e-7",
            "extra": "mean: 17.127775341821962 usec\nrounds: 53548"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 643.8286213046293,
            "unit": "iter/sec",
            "range": "stddev: 0.000009745437143147892",
            "extra": "mean: 1.5532083646322505 msec\nrounds: 639"
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
          "distinct": true,
          "id": "5fb5aee480e9440149e548ca399b3aa061218a04",
          "message": "src: fix APyFixed.__div__, closes #198",
          "timestamp": "2024-03-21T11:06:59+01:00",
          "tree_id": "ad81da3d6efed12f76dd5de6fed4d737efe06050",
          "url": "https://github.com/apytypes/apytypes/commit/5fb5aee480e9440149e548ca399b3aa061218a04"
        },
        "date": 1711015949673,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 350.56152605668274,
            "unit": "iter/sec",
            "range": "stddev: 0.000012546720184816764",
            "extra": "mean: 2.852566313390331 msec\nrounds: 351"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7713.704792031201,
            "unit": "iter/sec",
            "range": "stddev: 0.000003208696469111077",
            "extra": "mean: 129.6393920899164 usec\nrounds: 7636"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 76.88980253456806,
            "unit": "iter/sec",
            "range": "stddev: 0.0000344662564950449",
            "extra": "mean: 13.005625805195958 msec\nrounds: 77"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 71283.21950989874,
            "unit": "iter/sec",
            "range": "stddev: 6.917145255994334e-7",
            "extra": "mean: 14.028547067253815 usec\nrounds: 47464"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 660.699641939608,
            "unit": "iter/sec",
            "range": "stddev: 0.000007938402342258886",
            "extra": "mean: 1.5135470590907418 msec\nrounds: 660"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 8649.78118945537,
            "unit": "iter/sec",
            "range": "stddev: 0.0000027674630901702383",
            "extra": "mean: 115.60986088515895 usec\nrounds: 8590"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 88.57287714032795,
            "unit": "iter/sec",
            "range": "stddev: 0.00007928015362583575",
            "extra": "mean: 11.29013793258266 msec\nrounds: 89"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 71093.13391906735,
            "unit": "iter/sec",
            "range": "stddev: 6.553361510191602e-7",
            "extra": "mean: 14.066055958911633 usec\nrounds: 68461"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 750.9629310028134,
            "unit": "iter/sec",
            "range": "stddev: 0.000008325280589787227",
            "extra": "mean: 1.3316236510697406 msec\nrounds: 748"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 9468.70751121945,
            "unit": "iter/sec",
            "range": "stddev: 0.000003521969769579829",
            "extra": "mean: 105.61103496069578 usec\nrounds: 9382"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 136.2250956724424,
            "unit": "iter/sec",
            "range": "stddev: 0.00007201254142481632",
            "extra": "mean: 7.340791320892387 msec\nrounds: 134"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78941.5635054556,
            "unit": "iter/sec",
            "range": "stddev: 6.569044565944542e-7",
            "extra": "mean: 12.667598101612095 usec\nrounds: 64688"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 901.5716369799976,
            "unit": "iter/sec",
            "range": "stddev: 0.000007043818544270864",
            "extra": "mean: 1.1091742008984542 msec\nrounds: 891"
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
          "id": "6550e822faa3b418af34707dae8dae7edaab1107",
          "message": "Specialize conversion of numpy arrays to APyFloatArray",
          "timestamp": "2024-03-21T12:11:40+01:00",
          "tree_id": "af97207994a9f565b7b75c15f425f71655c29f1b",
          "url": "https://github.com/apytypes/apytypes/commit/6550e822faa3b418af34707dae8dae7edaab1107"
        },
        "date": 1711019711575,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 338.3396063500869,
            "unit": "iter/sec",
            "range": "stddev: 0.00003207996206178855",
            "extra": "mean: 2.9556102248498792 msec\nrounds: 338"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7506.489233129039,
            "unit": "iter/sec",
            "range": "stddev: 0.00000621928017188417",
            "extra": "mean: 133.21806891917106 usec\nrounds: 7429"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 74.82160996364095,
            "unit": "iter/sec",
            "range": "stddev: 0.00004562158130805436",
            "extra": "mean: 13.365122729729327 msec\nrounds: 74"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 72140.19814026664,
            "unit": "iter/sec",
            "range": "stddev: 8.433211576254834e-7",
            "extra": "mean: 13.861897052952894 usec\nrounds: 45975"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 668.3095754719047,
            "unit": "iter/sec",
            "range": "stddev: 0.000008287448514012506",
            "extra": "mean: 1.4963125424229977 msec\nrounds: 660"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 8393.04011850551,
            "unit": "iter/sec",
            "range": "stddev: 0.000003819798574197068",
            "extra": "mean: 119.14633861872483 usec\nrounds: 8387"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 82.96539686039557,
            "unit": "iter/sec",
            "range": "stddev: 0.00014142572536402526",
            "extra": "mean: 12.053217821433226 msec\nrounds: 84"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 72466.68483801199,
            "unit": "iter/sec",
            "range": "stddev: 6.523096935495725e-7",
            "extra": "mean: 13.79944456180581 usec\nrounds: 68130"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 723.5006812952155,
            "unit": "iter/sec",
            "range": "stddev: 0.000024656235170461036",
            "extra": "mean: 1.3821687053698328 msec\nrounds: 689"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 55270.65794477728,
            "unit": "iter/sec",
            "range": "stddev: 7.305712819516905e-7",
            "extra": "mean: 18.09278262978401 usec\nrounds: 53724"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 583.8928270183239,
            "unit": "iter/sec",
            "range": "stddev: 0.000007231174603462318",
            "extra": "mean: 1.7126430634651684 msec\nrounds: 583"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78518.80723285509,
            "unit": "iter/sec",
            "range": "stddev: 7.397317025597019e-7",
            "extra": "mean: 12.735802226776109 usec\nrounds: 69701"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 899.4825510749392,
            "unit": "iter/sec",
            "range": "stddev: 0.00001470763653764409",
            "extra": "mean: 1.111750304444412 msec\nrounds: 900"
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
          "id": "f02a6dea59557a08d0a5fec7b472e2941500e18b",
          "message": "Speed-up array conversion for APyFloat",
          "timestamp": "2024-03-21T14:16:09+01:00",
          "tree_id": "ac6b39dffe2c2b3c5beadd22b11b7c1fc96fdde7",
          "url": "https://github.com/apytypes/apytypes/commit/f02a6dea59557a08d0a5fec7b472e2941500e18b"
        },
        "date": 1711027217344,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 348.4167215978006,
            "unit": "iter/sec",
            "range": "stddev: 0.0001477824420134718",
            "extra": "mean: 2.8701263114299183 msec\nrounds: 350"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7998.98146235717,
            "unit": "iter/sec",
            "range": "stddev: 0.000005701044724511709",
            "extra": "mean: 125.01591667713608 usec\nrounds: 7921"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 78.86449014342843,
            "unit": "iter/sec",
            "range": "stddev: 0.00006545174868612207",
            "extra": "mean: 12.679978000001404 msec\nrounds: 79"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 80692.6670745929,
            "unit": "iter/sec",
            "range": "stddev: 5.613914544582336e-7",
            "extra": "mean: 12.392699810945555 usec\nrounds: 69363"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 731.442232142976,
            "unit": "iter/sec",
            "range": "stddev: 0.0000086304499669297",
            "extra": "mean: 1.3671619658468512 msec\nrounds: 732"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 8800.351800241368,
            "unit": "iter/sec",
            "range": "stddev: 0.0000037872397801557315",
            "extra": "mean: 113.6318209429506 usec\nrounds: 8757"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 88.45862507634901,
            "unit": "iter/sec",
            "range": "stddev: 0.0001384385450253582",
            "extra": "mean: 11.304720134830221 msec\nrounds: 89"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 81198.98464712739,
            "unit": "iter/sec",
            "range": "stddev: 8.491155408091035e-7",
            "extra": "mean: 12.315424932291656 usec\nrounds: 77137"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 806.4469000176326,
            "unit": "iter/sec",
            "range": "stddev: 0.000008320723175970447",
            "extra": "mean: 1.2400072465752368 msec\nrounds: 803"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 97568.3424507302,
            "unit": "iter/sec",
            "range": "stddev: 5.138322372579244e-7",
            "extra": "mean: 10.249226079708972 usec\nrounds: 89119"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 1079.9878976141301,
            "unit": "iter/sec",
            "range": "stddev: 0.000007171245700033979",
            "extra": "mean: 925.9363018874226 usec\nrounds: 1060"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78675.89351079985,
            "unit": "iter/sec",
            "range": "stddev: 5.903063419576414e-7",
            "extra": "mean: 12.710373602083461 usec\nrounds: 69028"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 902.1514168679853,
            "unit": "iter/sec",
            "range": "stddev: 0.000009678681964276189",
            "extra": "mean: 1.108461375000349 msec\nrounds: 904"
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
          "id": "bc76fdc0180387ded747b632f4283b7a7bf1e03d",
          "message": "Remove dead code",
          "timestamp": "2024-03-21T14:26:40+01:00",
          "tree_id": "49d0b8724c1964f2c690cc41a2a0cb7d29ca508b",
          "url": "https://github.com/apytypes/apytypes/commit/bc76fdc0180387ded747b632f4283b7a7bf1e03d"
        },
        "date": 1711027832553,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 351.4975768217502,
            "unit": "iter/sec",
            "range": "stddev: 0.00009085461153574444",
            "extra": "mean: 2.8449698260853595 msec\nrounds: 345"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7863.285466703609,
            "unit": "iter/sec",
            "range": "stddev: 0.000004266133237631347",
            "extra": "mean: 127.17330487801976 usec\nrounds: 5986"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 77.57730569810417,
            "unit": "iter/sec",
            "range": "stddev: 0.0001663722246046489",
            "extra": "mean: 12.890367756409951 msec\nrounds: 78"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 80115.86512924032,
            "unit": "iter/sec",
            "range": "stddev: 6.861523034713692e-7",
            "extra": "mean: 12.48192225580829 usec\nrounds: 67169"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 726.139368725527,
            "unit": "iter/sec",
            "range": "stddev: 0.000018425042895287325",
            "extra": "mean: 1.377146100417521 msec\nrounds: 717"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 8373.288132404963,
            "unit": "iter/sec",
            "range": "stddev: 0.000004856723322181045",
            "extra": "mean: 119.42739628533262 usec\nrounds: 8345"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 86.13961316619181,
            "unit": "iter/sec",
            "range": "stddev: 0.000028796438962907164",
            "extra": "mean: 11.609060724137096 msec\nrounds: 87"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 82006.20521239066,
            "unit": "iter/sec",
            "range": "stddev: 6.707936037406746e-7",
            "extra": "mean: 12.194199175660744 usec\nrounds: 79096"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 826.7725165527497,
            "unit": "iter/sec",
            "range": "stddev: 0.0000093577358810588",
            "extra": "mean: 1.2095225469873223 msec\nrounds: 830"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 97440.16529102594,
            "unit": "iter/sec",
            "range": "stddev: 7.104313893695469e-7",
            "extra": "mean: 10.262708370960635 usec\nrounds: 91150"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 1080.257210738051,
            "unit": "iter/sec",
            "range": "stddev: 0.000006159381478872335",
            "extra": "mean: 925.70546168054 usec\nrounds: 1083"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78610.42239105304,
            "unit": "iter/sec",
            "range": "stddev: 8.156540945994763e-7",
            "extra": "mean: 12.720959506176294 usec\nrounds: 69171"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 902.8555227617043,
            "unit": "iter/sec",
            "range": "stddev: 0.000013851033238689123",
            "extra": "mean: 1.107596924191309 msec\nrounds: 897"
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
          "id": "1879554678b31d0089d3f976add183a1b029fa30",
          "message": "src: APyFixed::operator/() specialize for single limb",
          "timestamp": "2024-03-21T14:34:43+01:00",
          "tree_id": "51245ab8c4db8f2314cda6d31af54d00c25044e6",
          "url": "https://github.com/apytypes/apytypes/commit/1879554678b31d0089d3f976add183a1b029fa30"
        },
        "date": 1711028651318,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 350.68090745452133,
            "unit": "iter/sec",
            "range": "stddev: 0.000013126459843539873",
            "extra": "mean: 2.8515952215895495 msec\nrounds: 352"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7886.693454889078,
            "unit": "iter/sec",
            "range": "stddev: 0.000011849676760816462",
            "extra": "mean: 126.79584996169534 usec\nrounds: 7758"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 78.69755752162764,
            "unit": "iter/sec",
            "range": "stddev: 0.00009275492844618922",
            "extra": "mean: 12.70687466666523 msec\nrounds: 78"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 79979.64546275033,
            "unit": "iter/sec",
            "range": "stddev: 9.022366359317879e-7",
            "extra": "mean: 12.503181205844925 usec\nrounds: 66499"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 727.8153452412544,
            "unit": "iter/sec",
            "range": "stddev: 0.00001734027406286606",
            "extra": "mean: 1.373974877746666 msec\nrounds: 728"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 8800.250524629324,
            "unit": "iter/sec",
            "range": "stddev: 0.0000031576722045815832",
            "extra": "mean: 113.63312864801891 usec\nrounds: 8737"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 90.99139451360303,
            "unit": "iter/sec",
            "range": "stddev: 0.00003822667129510392",
            "extra": "mean: 10.990050271737532 msec\nrounds: 92"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 82100.25665762558,
            "unit": "iter/sec",
            "range": "stddev: 0.0000010656572829947342",
            "extra": "mean: 12.180229888564163 usec\nrounds: 79221"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 790.7697726936457,
            "unit": "iter/sec",
            "range": "stddev: 0.00004203349320224435",
            "extra": "mean: 1.2645905730483868 msec\nrounds: 794"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 96720.63334130032,
            "unit": "iter/sec",
            "range": "stddev: 5.778377198840442e-7",
            "extra": "mean: 10.339055540210094 usec\nrounds: 88098"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 1070.9349211906817,
            "unit": "iter/sec",
            "range": "stddev: 0.000007443018612907522",
            "extra": "mean: 933.7635557613387 usec\nrounds: 1076"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 76701.4237534081,
            "unit": "iter/sec",
            "range": "stddev: 9.472387647455223e-7",
            "extra": "mean: 13.03756763648819 usec\nrounds: 68602"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 893.2287279172423,
            "unit": "iter/sec",
            "range": "stddev: 0.00003890886618907849",
            "extra": "mean: 1.119534077605988 msec\nrounds: 902"
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
          "distinct": true,
          "id": "16fa3b966d54c3882e6d9c770fbe9dad72203278",
          "message": "src: APyFixed.cc, explicit template instantiation",
          "timestamp": "2024-03-21T14:42:13+01:00",
          "tree_id": "7dbac5963ee33f6aa5c65d66c0cb57c87508bfe3",
          "url": "https://github.com/apytypes/apytypes/commit/16fa3b966d54c3882e6d9c770fbe9dad72203278"
        },
        "date": 1711028905048,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_multiplication_20",
            "value": 354.1603092133426,
            "unit": "iter/sec",
            "range": "stddev: 0.00003651036297822161",
            "extra": "mean: 2.8235800963162423 msec\nrounds: 353"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_20",
            "value": 7951.379505356236,
            "unit": "iter/sec",
            "range": "stddev: 0.0000047706759151491785",
            "extra": "mean: 125.76434055579621 usec\nrounds: 7846"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_addition_200",
            "value": 78.83870948183768,
            "unit": "iter/sec",
            "range": "stddev: 0.0001443701410667684",
            "extra": "mean: 12.684124417718598 msec\nrounds: 79"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_20",
            "value": 77716.30022339428,
            "unit": "iter/sec",
            "range": "stddev: 6.048416742786045e-7",
            "extra": "mean: 12.86731351242295 usec\nrounds: 65755"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_elementwise_multiplication_200",
            "value": 729.4840917995488,
            "unit": "iter/sec",
            "range": "stddev: 0.000027476159601989227",
            "extra": "mean: 1.370831812840663 msec\nrounds: 732"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_20",
            "value": 8799.134858378775,
            "unit": "iter/sec",
            "range": "stddev: 0.0000030970637785116207",
            "extra": "mean: 113.6475365015883 usec\nrounds: 8753"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_addition_200",
            "value": 91.78329247728279,
            "unit": "iter/sec",
            "range": "stddev: 0.00003584502320841339",
            "extra": "mean: 10.895229109889572 msec\nrounds: 91"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_20",
            "value": 80610.809929595,
            "unit": "iter/sec",
            "range": "stddev: 6.329014361239837e-7",
            "extra": "mean: 12.405284116031014 usec\nrounds: 77676"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_scalar_matrix_multiplication_200",
            "value": 797.1055623283168,
            "unit": "iter/sec",
            "range": "stddev: 0.000008587940384631446",
            "extra": "mean: 1.2545389811093977 msec\nrounds: 794"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_20",
            "value": 97299.87023722485,
            "unit": "iter/sec",
            "range": "stddev: 5.768187120883907e-7",
            "extra": "mean: 10.277505998331963 usec\nrounds: 89358"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_creation_200",
            "value": 1077.0487055873243,
            "unit": "iter/sec",
            "range": "stddev: 0.000016276414723913815",
            "extra": "mean: 928.4631185315718 usec\nrounds: 1063"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_20",
            "value": 78513.79086210621,
            "unit": "iter/sec",
            "range": "stddev: 6.53874981727695e-7",
            "extra": "mean: 12.736615937400096 usec\nrounds: 69999"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_float_matrix_to_numpy_200",
            "value": 897.7658106065429,
            "unit": "iter/sec",
            "range": "stddev: 0.00003016431089484563",
            "extra": "mean: 1.1138762338525525 msec\nrounds: 898"
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
          "id": "1085f94a22bc3a38032b3bf5229073b8456b1983",
          "message": "More fixed-point array benchmarks",
          "timestamp": "2024-03-16T15:46:49+01:00",
          "tree_id": "eab1295361478d03a914832f57e36855cbc57364",
          "url": "https://github.com/apytypes/apytypes/commit/1085f94a22bc3a38032b3bf5229073b8456b1983"
        },
        "date": 1710600653549,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4421659.285820647,
            "unit": "iter/sec",
            "range": "stddev: 1.0652266636168891e-8",
            "extra": "mean: 226.15944272478976 nsec\nrounds: 44538"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2758984.2219093884,
            "unit": "iter/sec",
            "range": "stddev: 2.9836905503554765e-8",
            "extra": "mean: 362.452235884067 nsec\nrounds: 194970"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2820581.5661528166,
            "unit": "iter/sec",
            "range": "stddev: 2.8964411810838302e-8",
            "extra": "mean: 354.5368132587587 nsec\nrounds: 191939"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2992835.8429045463,
            "unit": "iter/sec",
            "range": "stddev: 2.5857456727569575e-8",
            "extra": "mean: 334.13125627032287 nsec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2156089.779610783,
            "unit": "iter/sec",
            "range": "stddev: 3.727766691388584e-8",
            "extra": "mean: 463.8025788427745 nsec\nrounds: 131493"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2600160.688586949,
            "unit": "iter/sec",
            "range": "stddev: 8.005862281785275e-8",
            "extra": "mean: 384.5916155833398 nsec\nrounds: 191205"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4179181.5703774192,
            "unit": "iter/sec",
            "range": "stddev: 2.020416290863335e-8",
            "extra": "mean: 239.2813002161399 nsec\nrounds: 198413"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3939922.109912944,
            "unit": "iter/sec",
            "range": "stddev: 2.5120069778694817e-8",
            "extra": "mean: 253.81212422548228 nsec\nrounds: 195695"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3627483.1760418946,
            "unit": "iter/sec",
            "range": "stddev: 2.3560604770248307e-8",
            "extra": "mean: 275.6732289219061 nsec\nrounds: 199601"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3297769.228020308,
            "unit": "iter/sec",
            "range": "stddev: 2.3286843308536987e-8",
            "extra": "mean: 303.23528750988817 nsec\nrounds: 197668"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5123850.943746146,
            "unit": "iter/sec",
            "range": "stddev: 7.691539465127749e-9",
            "extra": "mean: 195.1657085615545 nsec\nrounds: 51661"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4901834.375975733,
            "unit": "iter/sec",
            "range": "stddev: 1.1583770416556891e-8",
            "extra": "mean: 204.0052607450836 nsec\nrounds: 49339"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8079384.796322129,
            "unit": "iter/sec",
            "range": "stddev: 2.4744382567333823e-8",
            "extra": "mean: 123.7718001072263 nsec\nrounds: 85456"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4881250.35268784,
            "unit": "iter/sec",
            "range": "stddev: 9.111724876746541e-9",
            "extra": "mean: 204.86554217597512 nsec\nrounds: 49412"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4659448.018763882,
            "unit": "iter/sec",
            "range": "stddev: 1.2698451199527909e-8",
            "extra": "mean: 214.61769633930876 nsec\nrounds: 46361"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5923822.339093374,
            "unit": "iter/sec",
            "range": "stddev: 8.612795873281878e-9",
            "extra": "mean: 168.80992419381204 nsec\nrounds: 58439"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5848055.441534529,
            "unit": "iter/sec",
            "range": "stddev: 1.1428004481930826e-8",
            "extra": "mean: 170.99701088631272 nsec\nrounds: 59981"
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
          "id": "3cdd779f89b4253f80f05a35d5f25be8fb8e9dca",
          "message": "Compute shift amount less often",
          "timestamp": "2024-03-16T18:19:28+01:00",
          "tree_id": "181c28f8026c39ffd5cf445e4725d749df290e55",
          "url": "https://github.com/apytypes/apytypes/commit/3cdd779f89b4253f80f05a35d5f25be8fb8e9dca"
        },
        "date": 1710609810927,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4056265.6393769784,
            "unit": "iter/sec",
            "range": "stddev: 2.0101782319967507e-8",
            "extra": "mean: 246.53217735356716 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2384395.1466744374,
            "unit": "iter/sec",
            "range": "stddev: 6.085224793312484e-8",
            "extra": "mean: 419.3935729967239 nsec\nrounds: 187653"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2928765.6488009053,
            "unit": "iter/sec",
            "range": "stddev: 2.9682480972574475e-8",
            "extra": "mean: 341.4407705886152 nsec\nrounds: 191939"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2886588.7491757805,
            "unit": "iter/sec",
            "range": "stddev: 3.007965038141061e-8",
            "extra": "mean: 346.42967422551646 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2259654.37104145,
            "unit": "iter/sec",
            "range": "stddev: 5.895510440892798e-8",
            "extra": "mean: 442.545555999833 nsec\nrounds: 185186"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2612438.2168617025,
            "unit": "iter/sec",
            "range": "stddev: 3.562053766773391e-8",
            "extra": "mean: 382.7841720985699 nsec\nrounds: 169435"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4013717.707975958,
            "unit": "iter/sec",
            "range": "stddev: 2.2848512785778752e-8",
            "extra": "mean: 249.1455734450119 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3808691.7311827014,
            "unit": "iter/sec",
            "range": "stddev: 2.1427731825569578e-8",
            "extra": "mean: 262.5573479241757 nsec\nrounds: 198453"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3551622.7187932627,
            "unit": "iter/sec",
            "range": "stddev: 2.269238634342819e-8",
            "extra": "mean: 281.56143801776227 nsec\nrounds: 198453"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3254382.3990503387,
            "unit": "iter/sec",
            "range": "stddev: 2.6011960557756636e-8",
            "extra": "mean: 307.2779647197406 nsec\nrounds: 191242"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4675491.145082337,
            "unit": "iter/sec",
            "range": "stddev: 1.1154058484219802e-8",
            "extra": "mean: 213.88127342554313 nsec\nrounds: 47062"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4624635.903634334,
            "unit": "iter/sec",
            "range": "stddev: 2.2249493204622035e-8",
            "extra": "mean: 216.23323886194171 nsec\nrounds: 43682"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8199586.376565994,
            "unit": "iter/sec",
            "range": "stddev: 1.610862950128862e-8",
            "extra": "mean: 121.95737127157047 nsec\nrounds: 82150"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4429576.92883456,
            "unit": "iter/sec",
            "range": "stddev: 1.1681783822412567e-8",
            "extra": "mean: 225.75519424673826 nsec\nrounds: 45123"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4205061.213695889,
            "unit": "iter/sec",
            "range": "stddev: 2.2020980488299948e-8",
            "extra": "mean: 237.8086665523324 nsec\nrounds: 197668"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5740814.287232794,
            "unit": "iter/sec",
            "range": "stddev: 1.1447374132996107e-8",
            "extra": "mean: 174.19131676560178 nsec\nrounds: 56297"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5786592.789520767,
            "unit": "iter/sec",
            "range": "stddev: 1.0683230010297817e-8",
            "extra": "mean: 172.81326617815444 nsec\nrounds: 57330"
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
          "id": "a78d22c8c9593f9c803cb11afde930b4e65a5d70",
          "message": "Inline scalar cast in array",
          "timestamp": "2024-03-16T19:17:31+01:00",
          "tree_id": "eeb97c983d61e9e692a2ea000ecf242953c9313e",
          "url": "https://github.com/apytypes/apytypes/commit/a78d22c8c9593f9c803cb11afde930b4e65a5d70"
        },
        "date": 1710613336628,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4187741.1458292985,
            "unit": "iter/sec",
            "range": "stddev: 1.7884161340494872e-8",
            "extra": "mean: 238.79221880647896 nsec\nrounds: 199601"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2570544.453942227,
            "unit": "iter/sec",
            "range": "stddev: 3.126995955799858e-8",
            "extra": "mean: 389.02264400307774 nsec\nrounds: 193799"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2986480.472342409,
            "unit": "iter/sec",
            "range": "stddev: 2.58433403985808e-8",
            "extra": "mean: 334.84230325999494 nsec\nrounds: 196464"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3207457.218962977,
            "unit": "iter/sec",
            "range": "stddev: 2.829373194268507e-8",
            "extra": "mean: 311.7734491010035 nsec\nrounds: 196079"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2343073.348814436,
            "unit": "iter/sec",
            "range": "stddev: 3.137953086223854e-8",
            "extra": "mean: 426.7898828288892 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2819743.845920474,
            "unit": "iter/sec",
            "range": "stddev: 2.6194924610271806e-8",
            "extra": "mean: 354.64214291906853 nsec\nrounds: 182449"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4175450.849660841,
            "unit": "iter/sec",
            "range": "stddev: 1.811862964396861e-8",
            "extra": "mean: 239.49509550115795 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4271497.679003894,
            "unit": "iter/sec",
            "range": "stddev: 1.851879076134935e-8",
            "extra": "mean: 234.10992470280812 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3799841.7491611196,
            "unit": "iter/sec",
            "range": "stddev: 2.110894736172912e-8",
            "extra": "mean: 263.16885439256663 nsec\nrounds: 199204"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3331430.5647060703,
            "unit": "iter/sec",
            "range": "stddev: 2.211866200855235e-8",
            "extra": "mean: 300.17134698659885 nsec\nrounds: 195313"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5120180.204502642,
            "unit": "iter/sec",
            "range": "stddev: 7.184811374605446e-9",
            "extra": "mean: 195.3056259857249 nsec\nrounds: 51319"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4607922.770464594,
            "unit": "iter/sec",
            "range": "stddev: 9.466607429879083e-9",
            "extra": "mean: 217.0175260769627 nsec\nrounds: 48219"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7948406.544842795,
            "unit": "iter/sec",
            "range": "stddev: 7.182437508753772e-9",
            "extra": "mean: 125.81138047710932 nsec\nrounds: 79726"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4547822.528136343,
            "unit": "iter/sec",
            "range": "stddev: 1.2052425326329148e-8",
            "extra": "mean: 219.88544931404792 nsec\nrounds: 45536"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4258564.420321296,
            "unit": "iter/sec",
            "range": "stddev: 1.5833864372119402e-8",
            "extra": "mean: 234.8209164637454 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5014627.498080658,
            "unit": "iter/sec",
            "range": "stddev: 1.0852133120500712e-8",
            "extra": "mean: 199.41660679330892 nsec\nrounds: 58066"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5615707.86917991,
            "unit": "iter/sec",
            "range": "stddev: 8.796315005806617e-9",
            "extra": "mean: 178.07194093693644 nsec\nrounds: 57498"
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
          "id": "0e8326562c018e28bef8d0facf1f1b0b82128c20",
          "message": "Remove unused arguments",
          "timestamp": "2024-03-16T19:30:41+01:00",
          "tree_id": "3eb7bbd376c47582af0b56edcd3bd0a27378db8e",
          "url": "https://github.com/apytypes/apytypes/commit/0e8326562c018e28bef8d0facf1f1b0b82128c20"
        },
        "date": 1710614067300,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 3867774.3385574133,
            "unit": "iter/sec",
            "range": "stddev: 1.903578741226674e-8",
            "extra": "mean: 258.54662461322 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2580707.1765029505,
            "unit": "iter/sec",
            "range": "stddev: 3.423823794632581e-8",
            "extra": "mean: 387.4906882521538 nsec\nrounds: 196117"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2948101.356083932,
            "unit": "iter/sec",
            "range": "stddev: 3.39769027473551e-8",
            "extra": "mean: 339.2013635950603 nsec\nrounds: 190477"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2891554.8348328983,
            "unit": "iter/sec",
            "range": "stddev: 3.207735683497555e-8",
            "extra": "mean: 345.8347004018519 nsec\nrounds: 190115"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2185687.40483799,
            "unit": "iter/sec",
            "range": "stddev: 3.6986423773113795e-8",
            "extra": "mean: 457.5219666756068 nsec\nrounds: 194553"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2615579.2022273676,
            "unit": "iter/sec",
            "range": "stddev: 3.3337738671606e-8",
            "extra": "mean: 382.3244959081902 nsec\nrounds: 196890"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4016375.3201249084,
            "unit": "iter/sec",
            "range": "stddev: 2.036012953752169e-8",
            "extra": "mean: 248.9807152706573 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3917105.8317335774,
            "unit": "iter/sec",
            "range": "stddev: 2.0286110230224196e-8",
            "extra": "mean: 255.29052391146817 nsec\nrounds: 197629"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3598714.559945502,
            "unit": "iter/sec",
            "range": "stddev: 2.046473639600919e-8",
            "extra": "mean: 277.87699839550487 nsec\nrounds: 198020"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3330223.280694464,
            "unit": "iter/sec",
            "range": "stddev: 2.4562930954541777e-8",
            "extra": "mean: 300.28016613716323 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5470682.078438211,
            "unit": "iter/sec",
            "range": "stddev: 9.442856986117717e-9",
            "extra": "mean: 182.79256327858909 nsec\nrounds: 55390"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 5022335.539510092,
            "unit": "iter/sec",
            "range": "stddev: 1.140495076261958e-8",
            "extra": "mean: 199.11055168116195 nsec\nrounds: 50899"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8233457.146931466,
            "unit": "iter/sec",
            "range": "stddev: 6.682432018220658e-9",
            "extra": "mean: 121.45566341749615 nsec\nrounds: 82625"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4564499.002161038,
            "unit": "iter/sec",
            "range": "stddev: 1.1471913430863683e-8",
            "extra": "mean: 219.08209411952794 nsec\nrounds: 46621"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4405298.992165735,
            "unit": "iter/sec",
            "range": "stddev: 9.157537949679875e-9",
            "extra": "mean: 226.99934823456087 nsec\nrounds: 43758"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5823643.103900773,
            "unit": "iter/sec",
            "range": "stddev: 7.828235112752372e-9",
            "extra": "mean: 171.7138193668255 nsec\nrounds: 58439"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5649707.12672343,
            "unit": "iter/sec",
            "range": "stddev: 9.186182057429297e-9",
            "extra": "mean: 177.00032542746206 nsec\nrounds: 56234"
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
          "id": "7bbc14782cc3e9012e2c2a2fc782850da70caed2",
          "message": "Add test",
          "timestamp": "2024-03-16T20:08:08+01:00",
          "tree_id": "9e2df6f83d929296daf4e20e9b411e0019de64cd",
          "url": "https://github.com/apytypes/apytypes/commit/7bbc14782cc3e9012e2c2a2fc782850da70caed2"
        },
        "date": 1710616318193,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4286778.082309241,
            "unit": "iter/sec",
            "range": "stddev: 7.772036386750425e-9",
            "extra": "mean: 233.2754298914757 nsec\nrounds: 43511"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2575640.895666696,
            "unit": "iter/sec",
            "range": "stddev: 2.7163184714688363e-8",
            "extra": "mean: 388.2528817128152 nsec\nrounds: 196464"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2849120.786071655,
            "unit": "iter/sec",
            "range": "stddev: 2.4649955128100443e-8",
            "extra": "mean: 350.98547063669685 nsec\nrounds: 188680"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2955905.153154442,
            "unit": "iter/sec",
            "range": "stddev: 2.8505690427313568e-8",
            "extra": "mean: 338.30584818758007 nsec\nrounds: 198808"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2281955.6164462506,
            "unit": "iter/sec",
            "range": "stddev: 3.066721441420735e-8",
            "extra": "mean: 438.2206177862937 nsec\nrounds: 190115"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2546017.4527794844,
            "unit": "iter/sec",
            "range": "stddev: 2.7921968819450997e-8",
            "extra": "mean: 392.77028478664084 nsec\nrounds: 193837"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3702675.5822097915,
            "unit": "iter/sec",
            "range": "stddev: 1.930729534558505e-8",
            "extra": "mean: 270.07497086847945 nsec\nrounds: 196503"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3848366.874331295,
            "unit": "iter/sec",
            "range": "stddev: 2.003919480768865e-8",
            "extra": "mean: 259.850485324091 nsec\nrounds: 192345"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3588839.535353436,
            "unit": "iter/sec",
            "range": "stddev: 2.6955133007072164e-8",
            "extra": "mean: 278.6416027103282 nsec\nrounds: 198060"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3397823.691432732,
            "unit": "iter/sec",
            "range": "stddev: 2.2239577783966947e-8",
            "extra": "mean: 294.30602962749026 nsec\nrounds: 193088"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5283983.162800135,
            "unit": "iter/sec",
            "range": "stddev: 7.575052363619705e-9",
            "extra": "mean: 189.25117079107233 nsec\nrounds: 53434"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4836886.405562612,
            "unit": "iter/sec",
            "range": "stddev: 9.670694982827439e-9",
            "extra": "mean: 206.7445699882471 nsec\nrounds: 48429"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7920659.121754447,
            "unit": "iter/sec",
            "range": "stddev: 6.04797211286713e-9",
            "extra": "mean: 126.25211925274549 nsec\nrounds: 79981"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4657037.306671109,
            "unit": "iter/sec",
            "range": "stddev: 8.711392087366606e-9",
            "extra": "mean: 214.72879303917776 nsec\nrounds: 46381"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4335264.964948049,
            "unit": "iter/sec",
            "range": "stddev: 2.4619126407022725e-8",
            "extra": "mean: 230.6664086475331 nsec\nrounds: 43989"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5755120.386954788,
            "unit": "iter/sec",
            "range": "stddev: 1.0281303480457212e-8",
            "extra": "mean: 173.75831134074494 nsec\nrounds: 57898"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5419805.912678265,
            "unit": "iter/sec",
            "range": "stddev: 1.1130212716109818e-8",
            "extra": "mean: 184.50845216814213 nsec\nrounds: 55329"
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
          "id": "cb733a73063b35d56a76fb9fcd535011749fcd08",
          "message": "Add early return for cast",
          "timestamp": "2024-03-16T21:27:41+01:00",
          "tree_id": "ae4553afcd999df438dddc2d3d0ff31514975ddb",
          "url": "https://github.com/apytypes/apytypes/commit/cb733a73063b35d56a76fb9fcd535011749fcd08"
        },
        "date": 1710621152416,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 3983428.7621413264,
            "unit": "iter/sec",
            "range": "stddev: 2.033856080319723e-8",
            "extra": "mean: 251.04001093330197 nsec\nrounds: 193837"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2580348.2411309592,
            "unit": "iter/sec",
            "range": "stddev: 6.483472921981842e-8",
            "extra": "mean: 387.5445895480138 nsec\nrounds: 186916"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2906690.7944357325,
            "unit": "iter/sec",
            "range": "stddev: 4.6486155127775506e-8",
            "extra": "mean: 344.03384147855036 nsec\nrounds: 199204"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2970934.1881107735,
            "unit": "iter/sec",
            "range": "stddev: 3.153481028953348e-8",
            "extra": "mean: 336.5944637892287 nsec\nrounds: 196464"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2244857.9062367342,
            "unit": "iter/sec",
            "range": "stddev: 5.329038698752931e-8",
            "extra": "mean: 445.4624932926796 nsec\nrounds: 186220"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2565628.5597246396,
            "unit": "iter/sec",
            "range": "stddev: 3.445831416062378e-8",
            "extra": "mean: 389.76803411772966 nsec\nrounds: 193424"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3970822.030178405,
            "unit": "iter/sec",
            "range": "stddev: 2.136362273019909e-8",
            "extra": "mean: 251.8370232662369 nsec\nrounds: 188680"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3809798.3889876422,
            "unit": "iter/sec",
            "range": "stddev: 2.3715096189547976e-8",
            "extra": "mean: 262.4810811222952 nsec\nrounds: 194932"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3640773.609037715,
            "unit": "iter/sec",
            "range": "stddev: 2.8381593151508027e-8",
            "extra": "mean: 274.6668997814098 nsec\nrounds: 191608"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3472225.945494137,
            "unit": "iter/sec",
            "range": "stddev: 2.62017315102166e-8",
            "extra": "mean: 287.99969117717615 nsec\nrounds: 200000"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4356931.153544427,
            "unit": "iter/sec",
            "range": "stddev: 1.2301816411874063e-8",
            "extra": "mean: 229.51934854115558 nsec\nrounds: 43625"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4476053.4060397595,
            "unit": "iter/sec",
            "range": "stddev: 8.565647314612535e-9",
            "extra": "mean: 223.41109662603745 nsec\nrounds: 44719"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8450966.49202624,
            "unit": "iter/sec",
            "range": "stddev: 6.87568494887246e-9",
            "extra": "mean: 118.32966098534872 nsec\nrounds: 83949"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4342480.30236947,
            "unit": "iter/sec",
            "range": "stddev: 1.1871463587756612e-8",
            "extra": "mean: 230.28314013413112 nsec\nrounds: 44030"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4030358.9757181425,
            "unit": "iter/sec",
            "range": "stddev: 2.174902400591464e-8",
            "extra": "mean: 248.11685659387481 nsec\nrounds: 197668"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5203051.292832272,
            "unit": "iter/sec",
            "range": "stddev: 9.52945298727932e-9",
            "extra": "mean: 192.19491481424083 nsec\nrounds: 56171"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5837766.880435176,
            "unit": "iter/sec",
            "range": "stddev: 1.0081311944102858e-8",
            "extra": "mean: 171.29837838357676 nsec\nrounds: 56456"
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
          "id": "6f11740433df726d742b3675ec23303d2ca5a661",
          "message": "Add early return for cast and no quantization casting for arrays",
          "timestamp": "2024-03-16T23:56:33+01:00",
          "tree_id": "4373ecb5b36dff857ebbb054130efae2a3c81e8a",
          "url": "https://github.com/apytypes/apytypes/commit/6f11740433df726d742b3675ec23303d2ca5a661"
        },
        "date": 1710630032793,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4107543.756830524,
            "unit": "iter/sec",
            "range": "stddev: 2.1465759749372644e-8",
            "extra": "mean: 243.45449718880167 nsec\nrounds: 196890"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2446976.710414119,
            "unit": "iter/sec",
            "range": "stddev: 4.2385008700156344e-8",
            "extra": "mean: 408.66755933740086 nsec\nrounds: 185874"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2879209.254991864,
            "unit": "iter/sec",
            "range": "stddev: 2.7158097986549162e-8",
            "extra": "mean: 347.3175832101486 nsec\nrounds: 191571"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3178393.802569733,
            "unit": "iter/sec",
            "range": "stddev: 2.6676572260953782e-8",
            "extra": "mean: 314.62432351570135 nsec\nrounds: 198453"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2349924.7980579054,
            "unit": "iter/sec",
            "range": "stddev: 3.894077813138854e-8",
            "extra": "mean: 425.54553270236573 nsec\nrounds: 191939"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2528220.7415917953,
            "unit": "iter/sec",
            "range": "stddev: 4.1134937805223686e-8",
            "extra": "mean: 395.53508265678215 nsec\nrounds: 195695"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4204696.288496553,
            "unit": "iter/sec",
            "range": "stddev: 1.856821353179878e-8",
            "extra": "mean: 237.82930594434424 nsec\nrounds: 197629"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4071263.0684003904,
            "unit": "iter/sec",
            "range": "stddev: 2.2046953741852224e-8",
            "extra": "mean: 245.62401967136887 nsec\nrounds: 193424"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3703777.2291421555,
            "unit": "iter/sec",
            "range": "stddev: 2.2463293700428877e-8",
            "extra": "mean: 269.99464010225137 nsec\nrounds: 193837"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3450107.5202579168,
            "unit": "iter/sec",
            "range": "stddev: 2.43496024243649e-8",
            "extra": "mean: 289.8460393273707 nsec\nrounds: 199601"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5285255.265022645,
            "unit": "iter/sec",
            "range": "stddev: 9.08710957080072e-9",
            "extra": "mean: 189.20562013680734 nsec\nrounds: 50513"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 5028832.022279207,
            "unit": "iter/sec",
            "range": "stddev: 9.984825188693455e-9",
            "extra": "mean: 198.85333126451403 nsec\nrounds: 49906"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8728809.066966103,
            "unit": "iter/sec",
            "range": "stddev: 6.658894277204528e-9",
            "extra": "mean: 114.56316575695678 nsec\nrounds: 86349"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4671194.824815886,
            "unit": "iter/sec",
            "range": "stddev: 9.3356694032829e-9",
            "extra": "mean: 214.0779902151636 nsec\nrounds: 47015"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4471805.432729039,
            "unit": "iter/sec",
            "range": "stddev: 9.316781205769469e-9",
            "extra": "mean: 223.6233250849747 nsec\nrounds: 45268"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5732180.606648423,
            "unit": "iter/sec",
            "range": "stddev: 8.052830029456464e-9",
            "extra": "mean: 174.453679781171 nsec\nrounds: 57498"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5401393.380801554,
            "unit": "iter/sec",
            "range": "stddev: 1.0575870390163137e-8",
            "extra": "mean: 185.1374135337947 nsec\nrounds: 59734"
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
          "id": "794f95feac30dbced9c758d57e8c8bf546703adf",
          "message": "src: add faster APyFixedArray add/sub",
          "timestamp": "2024-03-18T09:40:47+01:00",
          "tree_id": "14f6e7a9f1597c5186a5f0637ca03ad956fabcdd",
          "url": "https://github.com/apytypes/apytypes/commit/794f95feac30dbced9c758d57e8c8bf546703adf"
        },
        "date": 1710751787687,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 3940581.5622294773,
            "unit": "iter/sec",
            "range": "stddev: 2.20347060328534e-8",
            "extra": "mean: 253.7696490246975 nsec\nrounds: 198020"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2538886.1966129146,
            "unit": "iter/sec",
            "range": "stddev: 3.197208978953408e-8",
            "extra": "mean: 393.8735030085828 nsec\nrounds: 191571"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2962660.5676919944,
            "unit": "iter/sec",
            "range": "stddev: 3.28783919004845e-8",
            "extra": "mean: 337.5344482271119 nsec\nrounds: 196851"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3013937.7437745463,
            "unit": "iter/sec",
            "range": "stddev: 2.761902313571034e-8",
            "extra": "mean: 331.79185670499356 nsec\nrounds: 196890"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2446490.5951321265,
            "unit": "iter/sec",
            "range": "stddev: 3.21353021709377e-8",
            "extra": "mean: 408.7487611804959 nsec\nrounds: 189790"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2528591.8898659046,
            "unit": "iter/sec",
            "range": "stddev: 3.1723354896133746e-8",
            "extra": "mean: 395.4770257738249 nsec\nrounds: 195351"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4410916.653249995,
            "unit": "iter/sec",
            "range": "stddev: 1.8880672675617235e-8",
            "extra": "mean: 226.710246103439 nsec\nrounds: 194591"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4246921.980275451,
            "unit": "iter/sec",
            "range": "stddev: 2.4926900461126272e-8",
            "extra": "mean: 235.46465055023174 nsec\nrounds: 199601"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3919730.029606127,
            "unit": "iter/sec",
            "range": "stddev: 2.4910282660832305e-8",
            "extra": "mean: 255.11961090336942 nsec\nrounds: 196851"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3692810.180186948,
            "unit": "iter/sec",
            "range": "stddev: 2.3841536307126643e-8",
            "extra": "mean: 270.7964805135801 nsec\nrounds: 199243"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4482313.492269632,
            "unit": "iter/sec",
            "range": "stddev: 1.1448254818274972e-8",
            "extra": "mean: 223.099076341834 nsec\nrounds: 45309"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4077781.5152255823,
            "unit": "iter/sec",
            "range": "stddev: 1.953405902082781e-8",
            "extra": "mean: 245.23138286491368 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8108060.932314235,
            "unit": "iter/sec",
            "range": "stddev: 8.302302137584557e-9",
            "extra": "mean: 123.3340509338163 nsec\nrounds: 81480"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4749745.616659153,
            "unit": "iter/sec",
            "range": "stddev: 1.406918933650434e-8",
            "extra": "mean: 210.53759100125677 nsec\nrounds: 46318"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4679288.923567324,
            "unit": "iter/sec",
            "range": "stddev: 1.308168945540958e-8",
            "extra": "mean: 213.70768429440025 nsec\nrounds: 47126"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5941646.967119609,
            "unit": "iter/sec",
            "range": "stddev: 1.0695669996549162e-8",
            "extra": "mean: 168.30350331046668 nsec\nrounds: 58890"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 6148381.513310187,
            "unit": "iter/sec",
            "range": "stddev: 8.267692474093634e-9",
            "extra": "mean: 162.64442891761306 nsec\nrounds: 60972"
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
          "id": "d18aef6266a3990a9fcfd7bafd748d9aab5b9a81",
          "message": "Faster unary operations (and some binary)",
          "timestamp": "2024-03-18T11:03:44+01:00",
          "tree_id": "43144e49a14d27b7d4f4b682e571c77f3d7f4f91",
          "url": "https://github.com/apytypes/apytypes/commit/d18aef6266a3990a9fcfd7bafd748d9aab5b9a81"
        },
        "date": 1710756481354,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4109024.3719569105,
            "unit": "iter/sec",
            "range": "stddev: 2.111798567303456e-8",
            "extra": "mean: 243.3667726151028 nsec\nrounds: 189754"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2520137.453574695,
            "unit": "iter/sec",
            "range": "stddev: 3.5884254351637185e-8",
            "extra": "mean: 396.80375313720907 nsec\nrounds: 194213"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2985315.4336586734,
            "unit": "iter/sec",
            "range": "stddev: 3.703141556387523e-8",
            "extra": "mean: 334.9729776376011 nsec\nrounds: 193837"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3210138.3456475427,
            "unit": "iter/sec",
            "range": "stddev: 3.435857128003513e-8",
            "extra": "mean: 311.5130540575758 nsec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2385372.1887170225,
            "unit": "iter/sec",
            "range": "stddev: 3.861147387405867e-8",
            "extra": "mean: 419.22179051554735 nsec\nrounds: 196890"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2468741.6641904986,
            "unit": "iter/sec",
            "range": "stddev: 3.645618462093762e-8",
            "extra": "mean: 405.0646588524262 nsec\nrounds: 191608"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3795636.3753906516,
            "unit": "iter/sec",
            "range": "stddev: 2.2382087717151935e-8",
            "extra": "mean: 263.4604322172401 nsec\nrounds: 188680"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3875290.984476206,
            "unit": "iter/sec",
            "range": "stddev: 2.149345067614194e-8",
            "extra": "mean: 258.045138805283 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3477701.3862576433,
            "unit": "iter/sec",
            "range": "stddev: 3.007168743677325e-8",
            "extra": "mean: 287.5462522320804 nsec\nrounds: 189394"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3365268.701437363,
            "unit": "iter/sec",
            "range": "stddev: 2.769192934026659e-8",
            "extra": "mean: 297.1530919871097 nsec\nrounds: 196117"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4694621.272816589,
            "unit": "iter/sec",
            "range": "stddev: 1.2099015121119934e-8",
            "extra": "mean: 213.00972791785117 nsec\nrounds: 48478"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4905915.940189072,
            "unit": "iter/sec",
            "range": "stddev: 1.1873623729050186e-8",
            "extra": "mean: 203.83553493202564 nsec\nrounds: 49025"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7822073.793430046,
            "unit": "iter/sec",
            "range": "stddev: 7.2352719893113e-9",
            "extra": "mean: 127.84333495291723 nsec\nrounds: 78223"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4384624.960091353,
            "unit": "iter/sec",
            "range": "stddev: 1.2958252454420147e-8",
            "extra": "mean: 228.06967736167547 nsec\nrounds: 44942"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4304498.265625312,
            "unit": "iter/sec",
            "range": "stddev: 1.4799186136565634e-8",
            "extra": "mean: 232.3151127707293 nsec\nrounds: 43229"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5524191.1031817105,
            "unit": "iter/sec",
            "range": "stddev: 1.1710672633193302e-8",
            "extra": "mean: 181.0219779369625 nsec\nrounds: 59734"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5877149.616463531,
            "unit": "iter/sec",
            "range": "stddev: 8.416583769350397e-9",
            "extra": "mean: 170.15050921942836 nsec\nrounds: 59169"
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
          "id": "8624f1a830a788c51c318cf00d5e3a9111d50647",
          "message": "Use inline shift for APyFixed",
          "timestamp": "2024-03-18T12:24:47+01:00",
          "tree_id": "ce60e8d72d0599048f0325a173b54ba40c955da0",
          "url": "https://github.com/apytypes/apytypes/commit/8624f1a830a788c51c318cf00d5e3a9111d50647"
        },
        "date": 1710761327866,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4153304.0301967952,
            "unit": "iter/sec",
            "range": "stddev: 2.7317838964358487e-8",
            "extra": "mean: 240.77216421665108 nsec\nrounds: 194970"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2487170.5065106093,
            "unit": "iter/sec",
            "range": "stddev: 3.44915892786262e-8",
            "extra": "mean: 402.06330743398826 nsec\nrounds: 190115"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2887236.461496094,
            "unit": "iter/sec",
            "range": "stddev: 2.8068665589920047e-8",
            "extra": "mean: 346.3519574291307 nsec\nrounds: 189790"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2901169.508878823,
            "unit": "iter/sec",
            "range": "stddev: 2.9109121835791548e-8",
            "extra": "mean: 344.68858056715277 nsec\nrounds: 191571"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2364946.5013627843,
            "unit": "iter/sec",
            "range": "stddev: 3.553950635595823e-8",
            "extra": "mean: 422.8425460887892 nsec\nrounds: 195734"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2495779.01342284,
            "unit": "iter/sec",
            "range": "stddev: 3.6460717415138766e-8",
            "extra": "mean: 400.67650005138 nsec\nrounds: 192716"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3927281.239960813,
            "unit": "iter/sec",
            "range": "stddev: 2.1958435399016373e-8",
            "extra": "mean: 254.6290776997674 nsec\nrounds: 195734"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3882931.2469162433,
            "unit": "iter/sec",
            "range": "stddev: 2.3236058097850926e-8",
            "extra": "mean: 257.53739543942544 nsec\nrounds: 195313"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3654921.4126900695,
            "unit": "iter/sec",
            "range": "stddev: 2.625905515805092e-8",
            "extra": "mean: 273.6036940572894 nsec\nrounds: 197278"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3434241.1810213285,
            "unit": "iter/sec",
            "range": "stddev: 2.4595554604376488e-8",
            "extra": "mean: 291.18514026523223 nsec\nrounds: 196079"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4890296.83422856,
            "unit": "iter/sec",
            "range": "stddev: 1.3208309998716027e-8",
            "extra": "mean: 204.48656470105814 nsec\nrounds: 50083"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4674606.025823327,
            "unit": "iter/sec",
            "range": "stddev: 9.261838932494234e-9",
            "extra": "mean: 213.92177104887787 nsec\nrounds: 46927"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7706463.435242416,
            "unit": "iter/sec",
            "range": "stddev: 8.29196435844924e-9",
            "extra": "mean: 129.76120738170727 nsec\nrounds: 77316"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4594666.352587242,
            "unit": "iter/sec",
            "range": "stddev: 9.978242885816512e-9",
            "extra": "mean: 217.6436596831335 nsec\nrounds: 44441"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 3984480.880644824,
            "unit": "iter/sec",
            "range": "stddev: 2.43143376483926e-8",
            "extra": "mean: 250.9737227897511 nsec\nrounds: 199243"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5911820.359319487,
            "unit": "iter/sec",
            "range": "stddev: 1.0448366439908122e-8",
            "extra": "mean: 169.15263645042273 nsec\nrounds: 58886"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5787456.970110249,
            "unit": "iter/sec",
            "range": "stddev: 1.24172662345831e-8",
            "extra": "mean: 172.78746177545207 nsec\nrounds: 54873"
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
          "id": "1cfd03b3ce602906dafa7e688b2d03ef70f9f403",
          "message": "Reuse variables for floating-point array arithmetic",
          "timestamp": "2024-03-18T12:45:51+01:00",
          "tree_id": "a7bcef98986f1516c9269c25df3071bc76bb461b",
          "url": "https://github.com/apytypes/apytypes/commit/1cfd03b3ce602906dafa7e688b2d03ef70f9f403"
        },
        "date": 1710762581601,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 3950757.986004295,
            "unit": "iter/sec",
            "range": "stddev: 2.0207651779751495e-8",
            "extra": "mean: 253.11598522163223 nsec\nrounds: 189394"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2489147.633759344,
            "unit": "iter/sec",
            "range": "stddev: 3.374483763547983e-8",
            "extra": "mean: 401.74394898781316 nsec\nrounds: 94787"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2782737.333592854,
            "unit": "iter/sec",
            "range": "stddev: 2.7944762242641846e-8",
            "extra": "mean: 359.3583871277459 nsec\nrounds: 199601"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2785998.0206043515,
            "unit": "iter/sec",
            "range": "stddev: 2.751101899280285e-8",
            "extra": "mean: 358.93779988512665 nsec\nrounds: 197278"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2105665.8801420517,
            "unit": "iter/sec",
            "range": "stddev: 4.409271060638548e-8",
            "extra": "mean: 474.9091531713381 nsec\nrounds: 188324"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2502505.4586332,
            "unit": "iter/sec",
            "range": "stddev: 3.314669497901625e-8",
            "extra": "mean: 399.599527965242 nsec\nrounds: 189394"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3995374.6702763233,
            "unit": "iter/sec",
            "range": "stddev: 3.4030865703800366e-8",
            "extra": "mean: 250.2894177707912 nsec\nrounds: 92937"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4006931.365352191,
            "unit": "iter/sec",
            "range": "stddev: 2.2155671085663686e-8",
            "extra": "mean: 249.56753905169288 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3696148.867785371,
            "unit": "iter/sec",
            "range": "stddev: 2.2385651435728476e-8",
            "extra": "mean: 270.55187325278285 nsec\nrounds: 189754"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3293608.736392916,
            "unit": "iter/sec",
            "range": "stddev: 2.6611677085242262e-8",
            "extra": "mean: 303.61833479198987 nsec\nrounds: 195313"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4916947.178318663,
            "unit": "iter/sec",
            "range": "stddev: 1.4603659782221019e-8",
            "extra": "mean: 203.37822712625754 nsec\nrounds: 49981"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4760932.97800174,
            "unit": "iter/sec",
            "range": "stddev: 1.2934272381262185e-8",
            "extra": "mean: 210.04286441768812 nsec\nrounds: 48502"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8533417.623556107,
            "unit": "iter/sec",
            "range": "stddev: 8.636356654319975e-9",
            "extra": "mean: 117.1863424613896 nsec\nrounds: 82427"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4520645.780392496,
            "unit": "iter/sec",
            "range": "stddev: 1.4978403965952965e-8",
            "extra": "mean: 221.20733377017373 nsec\nrounds: 45870"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4259680.226340951,
            "unit": "iter/sec",
            "range": "stddev: 1.2094168428885712e-8",
            "extra": "mean: 234.75940607375347 nsec\nrounds: 43625"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5623725.096818261,
            "unit": "iter/sec",
            "range": "stddev: 9.169363621764144e-9",
            "extra": "mean: 177.81808014858453 nsec\nrounds: 56421"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5375332.946877304,
            "unit": "iter/sec",
            "range": "stddev: 1.097517226270513e-8",
            "extra": "mean: 186.03498795009884 nsec\nrounds: 52701"
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
          "id": "ce8522b7e8c05ebc181b8b92a8314f805a068c34",
          "message": "Add fast path for floating-point multiplication",
          "timestamp": "2024-03-18T13:53:20+01:00",
          "tree_id": "e43a5b86300e1757e76647e397115aae9b8e8b56",
          "url": "https://github.com/apytypes/apytypes/commit/ce8522b7e8c05ebc181b8b92a8314f805a068c34"
        },
        "date": 1710766641876,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4366857.745554592,
            "unit": "iter/sec",
            "range": "stddev: 1.3865364668263607e-8",
            "extra": "mean: 228.9976129902564 nsec\nrounds: 42438"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2555991.6358982115,
            "unit": "iter/sec",
            "range": "stddev: 4.2828523655015895e-8",
            "extra": "mean: 391.23758699174675 nsec\nrounds: 194932"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2992185.935622912,
            "unit": "iter/sec",
            "range": "stddev: 2.8919995937041727e-8",
            "extra": "mean: 334.203830081155 nsec\nrounds: 191571"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3051719.060619873,
            "unit": "iter/sec",
            "range": "stddev: 5.4918933542328924e-8",
            "extra": "mean: 327.6841610042825 nsec\nrounds: 190477"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2192227.036125627,
            "unit": "iter/sec",
            "range": "stddev: 4.751424384105745e-8",
            "extra": "mean: 456.1571331439744 nsec\nrounds: 198847"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2435150.6118538557,
            "unit": "iter/sec",
            "range": "stddev: 5.7944529623945703e-8",
            "extra": "mean: 410.65221803211205 nsec\nrounds: 191939"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4521830.878491327,
            "unit": "iter/sec",
            "range": "stddev: 1.5459530218819878e-8",
            "extra": "mean: 221.14935893697816 nsec\nrounds: 45206"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4446795.1874744715,
            "unit": "iter/sec",
            "range": "stddev: 1.0068117777540244e-8",
            "extra": "mean: 224.88105654535056 nsec\nrounds: 44920"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3877299.3094014386,
            "unit": "iter/sec",
            "range": "stddev: 2.1023318408575905e-8",
            "extra": "mean: 257.91147915127203 nsec\nrounds: 188324"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3637893.8477265094,
            "unit": "iter/sec",
            "range": "stddev: 2.1714721874281587e-8",
            "extra": "mean: 274.8843264420502 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5003416.158809484,
            "unit": "iter/sec",
            "range": "stddev: 9.05401194490556e-9",
            "extra": "mean: 199.86344694494545 nsec\nrounds: 50259"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4695377.289885104,
            "unit": "iter/sec",
            "range": "stddev: 8.913797163713702e-9",
            "extra": "mean: 212.9754305695692 nsec\nrounds: 45765"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8250629.326020342,
            "unit": "iter/sec",
            "range": "stddev: 8.644320109668357e-9",
            "extra": "mean: 121.20287562143152 nsec\nrounds: 83036"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4456757.16003269,
            "unit": "iter/sec",
            "range": "stddev: 9.882989116185698e-9",
            "extra": "mean: 224.37839085505584 nsec\nrounds: 46795"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4494610.886936315,
            "unit": "iter/sec",
            "range": "stddev: 9.73461897693384e-9",
            "extra": "mean: 222.48867035552365 nsec\nrounds: 43568"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5569134.425790826,
            "unit": "iter/sec",
            "range": "stddev: 1.0016309597747086e-8",
            "extra": "mean: 179.56111731988588 nsec\nrounds: 61200"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5631615.596202063,
            "unit": "iter/sec",
            "range": "stddev: 8.43241076613819e-9",
            "extra": "mean: 177.5689378859562 nsec\nrounds: 59344"
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
          "id": "4c6ca2566921c2fc9b46840887dee88ec93197c4",
          "message": "Add unary negation and specialize array/scalar operations",
          "timestamp": "2024-03-18T17:30:20+01:00",
          "tree_id": "cb4c776d6cce7c51122365014a72eeb43deb5473",
          "url": "https://github.com/apytypes/apytypes/commit/4c6ca2566921c2fc9b46840887dee88ec93197c4"
        },
        "date": 1710779656293,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4099159.924329249,
            "unit": "iter/sec",
            "range": "stddev: 2.093384816415177e-8",
            "extra": "mean: 243.95242402358824 nsec\nrounds: 195695"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2588795.8704300346,
            "unit": "iter/sec",
            "range": "stddev: 3.093860439946246e-8",
            "extra": "mean: 386.279973412543 nsec\nrounds: 196503"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3029229.5652007586,
            "unit": "iter/sec",
            "range": "stddev: 2.7082672444389953e-8",
            "extra": "mean: 330.11694177552044 nsec\nrounds: 193799"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3013995.900902942,
            "unit": "iter/sec",
            "range": "stddev: 2.5934459099292666e-8",
            "extra": "mean: 331.7854545524647 nsec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2346756.650601013,
            "unit": "iter/sec",
            "range": "stddev: 3.180477173291533e-8",
            "extra": "mean: 426.12002388229473 nsec\nrounds: 191205"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2662053.8933452885,
            "unit": "iter/sec",
            "range": "stddev: 2.74264671762527e-8",
            "extra": "mean: 375.64979525765165 nsec\nrounds: 187618"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3946543.7678220673,
            "unit": "iter/sec",
            "range": "stddev: 1.8699808164432677e-8",
            "extra": "mean: 253.38626880397496 nsec\nrounds: 198060"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4037014.0431724624,
            "unit": "iter/sec",
            "range": "stddev: 1.769934777040301e-8",
            "extra": "mean: 247.70783289476825 nsec\nrounds: 199243"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3640650.192766925,
            "unit": "iter/sec",
            "range": "stddev: 2.0481166747760163e-8",
            "extra": "mean: 274.67621085545335 nsec\nrounds: 193837"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3504424.55474208,
            "unit": "iter/sec",
            "range": "stddev: 2.048596652930539e-8",
            "extra": "mean: 285.35355359454576 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4919040.962077271,
            "unit": "iter/sec",
            "range": "stddev: 7.621589953824332e-9",
            "extra": "mean: 203.2916594330932 nsec\nrounds: 49535"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4714693.429047731,
            "unit": "iter/sec",
            "range": "stddev: 8.061279215370165e-9",
            "extra": "mean: 212.10286841535128 nsec\nrounds: 47148"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8675711.96097896,
            "unit": "iter/sec",
            "range": "stddev: 5.8028413871117715e-9",
            "extra": "mean: 115.26431542419381 nsec\nrounds: 86791"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4788366.314064247,
            "unit": "iter/sec",
            "range": "stddev: 9.077900506256273e-9",
            "extra": "mean: 208.8394943935672 nsec\nrounds: 47804"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4501152.5592488535,
            "unit": "iter/sec",
            "range": "stddev: 9.757377236366963e-9",
            "extra": "mean: 222.1653202899949 nsec\nrounds: 45084"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5917062.346071688,
            "unit": "iter/sec",
            "range": "stddev: 8.463575289230908e-9",
            "extra": "mean: 169.00278237966108 nsec\nrounds: 56139"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5413509.987805557,
            "unit": "iter/sec",
            "range": "stddev: 9.504252773100324e-9",
            "extra": "mean: 184.72303593278053 nsec\nrounds: 59415"
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
          "id": "386af56e4182053c65559ed6fbe59e22eff8b257",
          "message": "Special casing for double to fixed-point conversion",
          "timestamp": "2024-03-18T17:48:54+01:00",
          "tree_id": "22585c10515b3eaa1b1e87d7ca88e3d502dda7f9",
          "url": "https://github.com/apytypes/apytypes/commit/386af56e4182053c65559ed6fbe59e22eff8b257"
        },
        "date": 1710780763887,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4010646.013815422,
            "unit": "iter/sec",
            "range": "stddev: 2.2390274472562992e-8",
            "extra": "mean: 249.33639033599246 nsec\nrounds: 199204"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2639471.275143379,
            "unit": "iter/sec",
            "range": "stddev: 3.2015661215105004e-8",
            "extra": "mean: 378.863755562456 nsec\nrounds: 190877"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3026170.2824135893,
            "unit": "iter/sec",
            "range": "stddev: 2.6549984143229537e-8",
            "extra": "mean: 330.45067087316306 nsec\nrounds: 190477"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2898664.298965089,
            "unit": "iter/sec",
            "range": "stddev: 2.8334503376965848e-8",
            "extra": "mean: 344.9864823454211 nsec\nrounds: 190477"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2235221.158950043,
            "unit": "iter/sec",
            "range": "stddev: 3.766330633273253e-8",
            "extra": "mean: 447.3830233736933 nsec\nrounds: 185874"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2595648.65414428,
            "unit": "iter/sec",
            "range": "stddev: 3.122451149910041e-8",
            "extra": "mean: 385.26015391301644 nsec\nrounds: 194175"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3912532.024756991,
            "unit": "iter/sec",
            "range": "stddev: 2.1729882006063216e-8",
            "extra": "mean: 255.58896225584272 nsec\nrounds: 192716"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3930188.415624975,
            "unit": "iter/sec",
            "range": "stddev: 3.611286659442192e-8",
            "extra": "mean: 254.44072758047162 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3559793.9627777794,
            "unit": "iter/sec",
            "range": "stddev: 2.1941033223294976e-8",
            "extra": "mean: 280.9151345432522 nsec\nrounds: 197239"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3307099.851458752,
            "unit": "iter/sec",
            "range": "stddev: 2.416739144353e-8",
            "extra": "mean: 302.3797420446443 nsec\nrounds: 193837"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4537926.318847812,
            "unit": "iter/sec",
            "range": "stddev: 8.080850752010167e-9",
            "extra": "mean: 220.36497063573646 nsec\nrounds: 45286"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4373860.407300316,
            "unit": "iter/sec",
            "range": "stddev: 9.408283486282497e-9",
            "extra": "mean: 228.63098198813321 nsec\nrounds: 45744"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8139670.856744484,
            "unit": "iter/sec",
            "range": "stddev: 6.230886796621099e-9",
            "extra": "mean: 122.85509053126225 nsec\nrounds: 80302"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4611918.143058087,
            "unit": "iter/sec",
            "range": "stddev: 8.664052676374704e-9",
            "extra": "mean: 216.82952059875996 nsec\nrounds: 45640"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4429795.35267318,
            "unit": "iter/sec",
            "range": "stddev: 8.747525765463874e-9",
            "extra": "mean: 225.74406273566078 nsec\nrounds: 43931"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5459711.538128205,
            "unit": "iter/sec",
            "range": "stddev: 9.42873901810807e-9",
            "extra": "mean: 183.15985982344083 nsec\nrounds: 55359"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5740328.791010132,
            "unit": "iter/sec",
            "range": "stddev: 8.149797019914984e-9",
            "extra": "mean: 174.20604923642475 nsec\nrounds: 57561"
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
          "id": "6f1613718018f34beef7cc2801b3f740c395b1f8",
          "message": "src: fix memory out-of-bounds access bug, closes #179",
          "timestamp": "2024-03-18T18:18:25+01:00",
          "tree_id": "df4d00301812fd2d2eb9f063f7b2e3de1146007a",
          "url": "https://github.com/apytypes/apytypes/commit/6f1613718018f34beef7cc2801b3f740c395b1f8"
        },
        "date": 1710782938365,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 3906520.750276289,
            "unit": "iter/sec",
            "range": "stddev: 2.0639851684859665e-8",
            "extra": "mean: 255.98225733972208 nsec\nrounds: 195695"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2585166.4683267535,
            "unit": "iter/sec",
            "range": "stddev: 4.1082363471613225e-8",
            "extra": "mean: 386.82228485146425 nsec\nrounds: 194591"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3016798.12542502,
            "unit": "iter/sec",
            "range": "stddev: 3.346658942072494e-8",
            "extra": "mean: 331.47726776023575 nsec\nrounds: 192716"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2812813.6036338042,
            "unit": "iter/sec",
            "range": "stddev: 2.9210265738336104e-8",
            "extra": "mean: 355.5159142817332 nsec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2200489.9393480127,
            "unit": "iter/sec",
            "range": "stddev: 4.0289800752239036e-8",
            "extra": "mean: 454.4442499458628 nsec\nrounds: 195734"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2589482.043451597,
            "unit": "iter/sec",
            "range": "stddev: 3.278570136563003e-8",
            "extra": "mean: 386.1776151446263 nsec\nrounds: 192345"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3737345.1298204605,
            "unit": "iter/sec",
            "range": "stddev: 2.2730998054660568e-8",
            "extra": "mean: 267.56961566669884 nsec\nrounds: 189754"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3694498.4011131804,
            "unit": "iter/sec",
            "range": "stddev: 2.0805305656675558e-8",
            "extra": "mean: 270.6727386047094 nsec\nrounds: 197278"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3598446.218934119,
            "unit": "iter/sec",
            "range": "stddev: 3.69086742523285e-8",
            "extra": "mean: 277.8977200602737 nsec\nrounds: 196117"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3344560.9076772304,
            "unit": "iter/sec",
            "range": "stddev: 2.3709349133093195e-8",
            "extra": "mean: 298.9929104608685 nsec\nrounds: 198453"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4941250.5504035475,
            "unit": "iter/sec",
            "range": "stddev: 9.408924463704795e-9",
            "extra": "mean: 202.37791826165372 nsec\nrounds: 50184"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4793757.829469017,
            "unit": "iter/sec",
            "range": "stddev: 8.50887475773717e-9",
            "extra": "mean: 208.60461366088853 nsec\nrounds: 47756"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7798502.01323031,
            "unit": "iter/sec",
            "range": "stddev: 6.1530807429481205e-9",
            "extra": "mean: 128.2297546763888 nsec\nrounds: 77979"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4504308.440933251,
            "unit": "iter/sec",
            "range": "stddev: 9.124166732742328e-9",
            "extra": "mean: 222.00966321759114 nsec\nrounds: 45786"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4231714.096482595,
            "unit": "iter/sec",
            "range": "stddev: 1.8011906625880234e-8",
            "extra": "mean: 236.3108606113309 nsec\nrounds: 196079"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5653047.828157889,
            "unit": "iter/sec",
            "range": "stddev: 3.578395165950122e-8",
            "extra": "mean: 176.89572605755478 nsec\nrounds: 56552"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5576425.665041596,
            "unit": "iter/sec",
            "range": "stddev: 1.0409710609305114e-8",
            "extra": "mean: 179.32633914027923 nsec\nrounds: 55670"
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
          "id": "12af9d17ddfa5ee0d8bd3076a5cbdcd2a2b13d31",
          "message": "Add optmized two's overflow for single limb values",
          "timestamp": "2024-03-19T10:12:29+01:00",
          "tree_id": "5ecc3fa640bf11351eec27c66bf59994e2f9e629",
          "url": "https://github.com/apytypes/apytypes/commit/12af9d17ddfa5ee0d8bd3076a5cbdcd2a2b13d31"
        },
        "date": 1710839784050,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4199578.253610882,
            "unit": "iter/sec",
            "range": "stddev: 1.3181613799704307e-8",
            "extra": "mean: 238.11914902174547 nsec\nrounds: 41940"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2578329.9451339785,
            "unit": "iter/sec",
            "range": "stddev: 3.4983648799027134e-8",
            "extra": "mean: 387.84795634373825 nsec\nrounds: 194175"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2765758.8019021116,
            "unit": "iter/sec",
            "range": "stddev: 7.104384651979349e-8",
            "extra": "mean: 361.56442829077764 nsec\nrounds: 189036"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2885656.331770311,
            "unit": "iter/sec",
            "range": "stddev: 3.249580975525027e-8",
            "extra": "mean: 346.5416130778735 nsec\nrounds: 189394"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2171591.749554821,
            "unit": "iter/sec",
            "range": "stddev: 4.2954908491024695e-8",
            "extra": "mean: 460.49171084047845 nsec\nrounds: 192716"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2527034.3521664455,
            "unit": "iter/sec",
            "range": "stddev: 3.616169460161066e-8",
            "extra": "mean: 395.7207780506897 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3904835.642465933,
            "unit": "iter/sec",
            "range": "stddev: 2.2567077443468792e-8",
            "extra": "mean: 256.09272490881636 nsec\nrounds: 190513"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3889423.276840901,
            "unit": "iter/sec",
            "range": "stddev: 2.2959900195177603e-8",
            "extra": "mean: 257.10752695794207 nsec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3664612.948094528,
            "unit": "iter/sec",
            "range": "stddev: 2.248300548020465e-8",
            "extra": "mean: 272.88011426125865 nsec\nrounds: 198413"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3310197.9562744005,
            "unit": "iter/sec",
            "range": "stddev: 2.847562604580229e-8",
            "extra": "mean: 302.0967365727632 nsec\nrounds: 189072"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4983345.915125167,
            "unit": "iter/sec",
            "range": "stddev: 1.1287097981450409e-8",
            "extra": "mean: 200.66838967866065 nsec\nrounds: 50108"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4715312.927414858,
            "unit": "iter/sec",
            "range": "stddev: 1.1571176102129416e-8",
            "extra": "mean: 212.075002315532 nsec\nrounds: 47484"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7797825.791306492,
            "unit": "iter/sec",
            "range": "stddev: 6.820850199645784e-9",
            "extra": "mean: 128.24087466979043 nsec\nrounds: 77858"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4470622.166666502,
            "unit": "iter/sec",
            "range": "stddev: 1.5296378893590843e-8",
            "extra": "mean: 223.682512795671 nsec\nrounds: 44739"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4404504.446862932,
            "unit": "iter/sec",
            "range": "stddev: 1.0122494900562263e-8",
            "extra": "mean: 227.04029751002523 nsec\nrounds: 43360"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5893973.500968515,
            "unit": "iter/sec",
            "range": "stddev: 1.0102889711403562e-8",
            "extra": "mean: 169.6648279527582 nsec\nrounds: 59025"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5858162.513941372,
            "unit": "iter/sec",
            "range": "stddev: 8.3941357352325e-9",
            "extra": "mean: 170.70199019233732 nsec\nrounds: 59949"
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
          "id": "e0905d02079c6768e085064ac9a7b9292781ede5",
          "message": "Increase performance for fixed-point array negation",
          "timestamp": "2024-03-19T10:41:59+01:00",
          "tree_id": "537ea6009f257c3640ed92077a3a43073f449e4c",
          "url": "https://github.com/apytypes/apytypes/commit/e0905d02079c6768e085064ac9a7b9292781ede5"
        },
        "date": 1710841561957,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4142053.7358602863,
            "unit": "iter/sec",
            "range": "stddev: 1.875183533982888e-8",
            "extra": "mean: 241.42612910664684 nsec\nrounds: 195734"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2502251.5873238584,
            "unit": "iter/sec",
            "range": "stddev: 3.1569466858878226e-8",
            "extra": "mean: 399.64007019359263 nsec\nrounds: 186568"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2836160.059272,
            "unit": "iter/sec",
            "range": "stddev: 2.993555561883898e-8",
            "extra": "mean: 352.58940930742466 nsec\nrounds: 198808"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2788984.539116475,
            "unit": "iter/sec",
            "range": "stddev: 5.948480848784875e-8",
            "extra": "mean: 358.55343978247026 nsec\nrounds: 188006"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2221258.8035052284,
            "unit": "iter/sec",
            "range": "stddev: 4.050117417262896e-8",
            "extra": "mean: 450.19517690665555 nsec\nrounds: 198413"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2813440.372097867,
            "unit": "iter/sec",
            "range": "stddev: 2.9004623818498806e-8",
            "extra": "mean: 355.4367136825801 nsec\nrounds: 182782"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 3962609.9496248467,
            "unit": "iter/sec",
            "range": "stddev: 2.2984794383273843e-8",
            "extra": "mean: 252.35892826004678 nsec\nrounds: 197278"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3890695.1139250644,
            "unit": "iter/sec",
            "range": "stddev: 2.3054344038927943e-8",
            "extra": "mean: 257.0234805655116 nsec\nrounds: 192308"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3646913.2927451944,
            "unit": "iter/sec",
            "range": "stddev: 2.493235132237512e-8",
            "extra": "mean: 274.2044901342075 nsec\nrounds: 196079"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3333614.4107202096,
            "unit": "iter/sec",
            "range": "stddev: 2.4021110122841568e-8",
            "extra": "mean: 299.9747051682515 nsec\nrounds: 195734"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5352706.704575927,
            "unit": "iter/sec",
            "range": "stddev: 1.2277900072717463e-8",
            "extra": "mean: 186.82137004538993 nsec\nrounds: 55115"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 5017859.606646493,
            "unit": "iter/sec",
            "range": "stddev: 9.690377458550875e-9",
            "extra": "mean: 199.28815837639195 nsec\nrounds: 50184"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7599424.016925489,
            "unit": "iter/sec",
            "range": "stddev: 8.698183425169389e-9",
            "extra": "mean: 131.588920130383 nsec\nrounds: 75787"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4793473.023277954,
            "unit": "iter/sec",
            "range": "stddev: 1.0045808603560693e-8",
            "extra": "mean: 208.6170079906736 nsec\nrounds: 48175"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4507276.595081651,
            "unit": "iter/sec",
            "range": "stddev: 1.0333712973957823e-8",
            "extra": "mean: 221.86346431259253 nsec\nrounds: 47106"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5463155.58823257,
            "unit": "iter/sec",
            "range": "stddev: 1.017035487795212e-8",
            "extra": "mean: 183.04439327230133 nsec\nrounds: 55544"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5542423.348119584,
            "unit": "iter/sec",
            "range": "stddev: 8.252582524069704e-9",
            "extra": "mean: 180.42649166079244 nsec\nrounds: 55636"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "committer": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "distinct": false,
          "id": "8fdcf13ac465a6ef94850f84f0c80dc9839539e0",
          "message": "Fix exception in multiplication",
          "timestamp": "2024-03-20T09:05:57+01:00",
          "tree_id": "dab4b4d2366191e3f34569bb6e583ce6d6aafc79",
          "url": "https://github.com/apytypes/apytypes/commit/8fdcf13ac465a6ef94850f84f0c80dc9839539e0"
        },
        "date": 1710922563886,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 3802718.493894587,
            "unit": "iter/sec",
            "range": "stddev: 2.220429035335763e-8",
            "extra": "mean: 262.9697679712913 nsec\nrounds: 188715"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2544990.0085618882,
            "unit": "iter/sec",
            "range": "stddev: 3.5239921890664734e-8",
            "extra": "mean: 392.9288510507085 nsec\nrounds: 191571"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2718409.456573812,
            "unit": "iter/sec",
            "range": "stddev: 1.7573017284951445e-7",
            "extra": "mean: 367.86216939532403 nsec\nrounds: 194553"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2897677.737724842,
            "unit": "iter/sec",
            "range": "stddev: 3.360463742301259e-8",
            "extra": "mean: 345.1039385715955 nsec\nrounds: 191939"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2328035.92284442,
            "unit": "iter/sec",
            "range": "stddev: 3.3778703940545585e-8",
            "extra": "mean: 429.5466363672413 nsec\nrounds: 190840"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2619089.7342529814,
            "unit": "iter/sec",
            "range": "stddev: 3.284272034167751e-8",
            "extra": "mean: 381.8120421464276 nsec\nrounds: 197629"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4061884.5695581115,
            "unit": "iter/sec",
            "range": "stddev: 2.190829165436803e-8",
            "extra": "mean: 246.1911417902952 nsec\nrounds: 190877"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 3910357.796210267,
            "unit": "iter/sec",
            "range": "stddev: 2.118013739119533e-8",
            "extra": "mean: 255.7310742687804 nsec\nrounds: 194970"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3687333.5203486886,
            "unit": "iter/sec",
            "range": "stddev: 3.0453076691834666e-8",
            "extra": "mean: 271.1986844916649 nsec\nrounds: 188680"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3281700.6073586303,
            "unit": "iter/sec",
            "range": "stddev: 1.609272261877571e-7",
            "extra": "mean: 304.72005817897366 nsec\nrounds: 198453"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 4993736.385584835,
            "unit": "iter/sec",
            "range": "stddev: 8.727495981184234e-9",
            "extra": "mean: 200.25085883312744 nsec\nrounds: 50033"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 5102091.74835449,
            "unit": "iter/sec",
            "range": "stddev: 1.519295570938647e-8",
            "extra": "mean: 195.99804341472642 nsec\nrounds: 51825"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8464204.488259543,
            "unit": "iter/sec",
            "range": "stddev: 7.934869882038929e-9",
            "extra": "mean: 118.14459366938948 nsec\nrounds: 84303"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4681160.495363915,
            "unit": "iter/sec",
            "range": "stddev: 8.952644094251765e-9",
            "extra": "mean: 213.6222419612017 nsec\nrounds: 46883"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4555550.379237013,
            "unit": "iter/sec",
            "range": "stddev: 1.3374535247061369e-8",
            "extra": "mean: 219.51244454625623 nsec\nrounds: 46254"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5751256.2603626335,
            "unit": "iter/sec",
            "range": "stddev: 2.0108537320091765e-8",
            "extra": "mean: 173.87505524523203 nsec\nrounds: 56294"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5956482.7219194,
            "unit": "iter/sec",
            "range": "stddev: 1.1038894949114817e-8",
            "extra": "mean: 167.88431137730723 nsec\nrounds: 54660"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "committer": {
            "email": "40732757+Theodor-Lindberg@users.noreply.github.com",
            "name": "Theodor-Lindberg",
            "username": "Theodor-Lindberg"
          },
          "distinct": true,
          "id": "945083a637e09d72f4d413cef4e83360f300d646",
          "message": "Handle long formats in APyFloat power function",
          "timestamp": "2024-03-20T10:56:32+01:00",
          "tree_id": "d363a7dae42731cbd84aeadab196f860388baf8f",
          "url": "https://github.com/apytypes/apytypes/commit/945083a637e09d72f4d413cef4e83360f300d646"
        },
        "date": 1710928825126,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4093342.617414442,
            "unit": "iter/sec",
            "range": "stddev: 2.2151606858714897e-8",
            "extra": "mean: 244.29912017279818 nsec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2598234.774562538,
            "unit": "iter/sec",
            "range": "stddev: 3.0469399776058936e-8",
            "extra": "mean: 384.8766900474906 nsec\nrounds: 198060"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2956236.898176189,
            "unit": "iter/sec",
            "range": "stddev: 2.6825392846528062e-8",
            "extra": "mean: 338.2678839496846 nsec\nrounds: 194970"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 2962530.6625094763,
            "unit": "iter/sec",
            "range": "stddev: 2.8389826881229906e-8",
            "extra": "mean: 337.54924890905454 nsec\nrounds: 197668"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2306815.9898840776,
            "unit": "iter/sec",
            "range": "stddev: 5.6058131816465676e-8",
            "extra": "mean: 433.4979488547074 nsec\nrounds: 198060"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2634451.8190265,
            "unit": "iter/sec",
            "range": "stddev: 3.692067713001452e-8",
            "extra": "mean: 379.58560971880405 nsec\nrounds: 184843"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4472547.691300469,
            "unit": "iter/sec",
            "range": "stddev: 1.1097774119604935e-8",
            "extra": "mean: 223.58621283008247 nsec\nrounds: 44862"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4413183.190764148,
            "unit": "iter/sec",
            "range": "stddev: 1.000917536657885e-8",
            "extra": "mean: 226.59381149024827 nsec\nrounds: 44560"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3786178.2521901974,
            "unit": "iter/sec",
            "range": "stddev: 2.0878746631702944e-8",
            "extra": "mean: 264.1185737680373 nsec\nrounds: 192308"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3575272.886863734,
            "unit": "iter/sec",
            "range": "stddev: 2.3709140465214697e-8",
            "extra": "mean: 279.6989297443205 nsec\nrounds: 196890"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5374440.5199099565,
            "unit": "iter/sec",
            "range": "stddev: 1.6215494369003983e-8",
            "extra": "mean: 186.06587909860053 nsec\nrounds: 54425"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4935361.047233186,
            "unit": "iter/sec",
            "range": "stddev: 1.002974729012746e-8",
            "extra": "mean: 202.61942144241866 nsec\nrounds: 51110"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8086650.654697857,
            "unit": "iter/sec",
            "range": "stddev: 7.2833659096839215e-9",
            "extra": "mean: 123.66059110265503 nsec\nrounds: 81018"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4928398.608138694,
            "unit": "iter/sec",
            "range": "stddev: 1.11558147032595e-8",
            "extra": "mean: 202.90566561488913 nsec\nrounds: 48196"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4698011.522138956,
            "unit": "iter/sec",
            "range": "stddev: 1.3175812339379594e-8",
            "extra": "mean: 212.8560126529434 nsec\nrounds: 45830"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 6005023.725794142,
            "unit": "iter/sec",
            "range": "stddev: 8.485863005345589e-9",
            "extra": "mean: 166.52723547198642 nsec\nrounds: 59663"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5747992.713635094,
            "unit": "iter/sec",
            "range": "stddev: 1.5314997699258597e-8",
            "extra": "mean: 173.97377655471143 nsec\nrounds: 56488"
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
          "id": "c478d297ca4154ef10258fa8f0e5949310cc8476",
          "message": "Restructure code for readability",
          "timestamp": "2024-03-20T12:11:04+01:00",
          "tree_id": "ab5740e9d48836c176f83823695ab1ea4f7b0e73",
          "url": "https://github.com/apytypes/apytypes/commit/c478d297ca4154ef10258fa8f0e5949310cc8476"
        },
        "date": 1710933298165,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4426404.533261048,
            "unit": "iter/sec",
            "range": "stddev: 1.0539129580236586e-8",
            "extra": "mean: 225.91699255813728 nsec\nrounds: 44759"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2542963.677921883,
            "unit": "iter/sec",
            "range": "stddev: 3.149774434013291e-8",
            "extra": "mean: 393.24195177537194 nsec\nrounds: 190840"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3152686.7000425523,
            "unit": "iter/sec",
            "range": "stddev: 2.8425704798862694e-8",
            "extra": "mean: 317.1897797476999 nsec\nrounds: 193799"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3192229.25968649,
            "unit": "iter/sec",
            "range": "stddev: 2.5413110847017882e-8",
            "extra": "mean: 313.26070863037273 nsec\nrounds: 195695"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2323693.6513221976,
            "unit": "iter/sec",
            "range": "stddev: 3.8616487666719366e-8",
            "extra": "mean: 430.3493274300684 nsec\nrounds: 193462"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2636856.958085385,
            "unit": "iter/sec",
            "range": "stddev: 3.1201689958204943e-8",
            "extra": "mean: 379.2393807839077 nsec\nrounds: 187618"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4134906.785869003,
            "unit": "iter/sec",
            "range": "stddev: 1.798866970421099e-8",
            "extra": "mean: 241.84342036861813 nsec\nrounds: 194932"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4010439.6538365493,
            "unit": "iter/sec",
            "range": "stddev: 1.9906975386541546e-8",
            "extra": "mean: 249.3492201144191 nsec\nrounds: 196503"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3736802.469525368,
            "unit": "iter/sec",
            "range": "stddev: 1.9935333320854413e-8",
            "extra": "mean: 267.6084722579156 nsec\nrounds: 193799"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3472231.4667377425,
            "unit": "iter/sec",
            "range": "stddev: 2.0834103244643655e-8",
            "extra": "mean: 287.99923322473796 nsec\nrounds: 189394"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5789253.216628276,
            "unit": "iter/sec",
            "range": "stddev: 1.0963782462368553e-8",
            "extra": "mean: 172.73385056429584 nsec\nrounds: 57202"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 5100939.637952163,
            "unit": "iter/sec",
            "range": "stddev: 1.2122317668263526e-8",
            "extra": "mean: 196.04231200064612 nsec\nrounds: 51449"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 6663280.854604437,
            "unit": "iter/sec",
            "range": "stddev: 6.885154022540208e-9",
            "extra": "mean: 150.07621948110886 nsec\nrounds: 67623"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4756945.569955731,
            "unit": "iter/sec",
            "range": "stddev: 1.2498884963500746e-8",
            "extra": "mean: 210.21892836361394 nsec\nrounds: 47964"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4468115.247447399,
            "unit": "iter/sec",
            "range": "stddev: 8.198549352420187e-9",
            "extra": "mean: 223.8080140326367 nsec\nrounds: 44759"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5893311.4630009085,
            "unit": "iter/sec",
            "range": "stddev: 1.0166384546858266e-8",
            "extra": "mean: 169.68388762040865 nsec\nrounds: 57894"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5857627.634684581,
            "unit": "iter/sec",
            "range": "stddev: 8.162217046020981e-9",
            "extra": "mean: 170.71757755284315 nsec\nrounds: 59411"
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
          "id": "68cddb2248c5f525387429bbf6d8ab139744fdfe",
          "message": "Add test for long integers and APyFloat",
          "timestamp": "2024-03-20T12:41:56+01:00",
          "tree_id": "67d63d3b788f6fbeb6a96785d86e2fc9a357b73a",
          "url": "https://github.com/apytypes/apytypes/commit/68cddb2248c5f525387429bbf6d8ab139744fdfe"
        },
        "date": 1710935177901,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4153280.6345201177,
            "unit": "iter/sec",
            "range": "stddev: 5.507581860643192e-8",
            "extra": "mean: 240.77352050043152 nsec\nrounds: 190477"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2672851.7380674216,
            "unit": "iter/sec",
            "range": "stddev: 4.638952552012699e-8",
            "extra": "mean: 374.13223702524107 nsec\nrounds: 130651"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2922779.0515247975,
            "unit": "iter/sec",
            "range": "stddev: 2.7643192279680674e-8",
            "extra": "mean: 342.14012840906815 nsec\nrounds: 192345"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3107313.7931003934,
            "unit": "iter/sec",
            "range": "stddev: 2.6070054792695193e-8",
            "extra": "mean: 321.8213758199899 nsec\nrounds: 193799"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2207447.192172213,
            "unit": "iter/sec",
            "range": "stddev: 3.44572357138026e-8",
            "extra": "mean: 453.01196945777855 nsec\nrounds: 184163"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2458224.7839542814,
            "unit": "iter/sec",
            "range": "stddev: 3.3411477080660854e-8",
            "extra": "mean: 406.79762344254215 nsec\nrounds: 185874"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4485647.999572656,
            "unit": "iter/sec",
            "range": "stddev: 9.777044141801626e-9",
            "extra": "mean: 222.93323062690183 nsec\nrounds: 44106"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4309755.799422618,
            "unit": "iter/sec",
            "range": "stddev: 8.194420965810307e-9",
            "extra": "mean: 232.0317081849636 nsec\nrounds: 44679"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3752406.104435207,
            "unit": "iter/sec",
            "range": "stddev: 1.821158459476214e-8",
            "extra": "mean: 266.4956756191788 nsec\nrounds: 197278"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3452624.297592255,
            "unit": "iter/sec",
            "range": "stddev: 2.057535814999762e-8",
            "extra": "mean: 289.63475716084196 nsec\nrounds: 190840"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5371424.793739227,
            "unit": "iter/sec",
            "range": "stddev: 8.902581803076082e-9",
            "extra": "mean: 186.17034369834914 nsec\nrounds: 54903"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4789208.478671454,
            "unit": "iter/sec",
            "range": "stddev: 9.223469385524854e-9",
            "extra": "mean: 208.8027707404509 nsec\nrounds: 48857"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 7905360.058440413,
            "unit": "iter/sec",
            "range": "stddev: 6.615925608642125e-9",
            "extra": "mean: 126.49645210431206 nsec\nrounds: 79340"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4526197.84770956,
            "unit": "iter/sec",
            "range": "stddev: 8.859809040457063e-9",
            "extra": "mean: 220.93598946544176 nsec\nrounds: 46126"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4197363.208563272,
            "unit": "iter/sec",
            "range": "stddev: 8.349050116710983e-9",
            "extra": "mean: 238.2448099701756 nsec\nrounds: 42730"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5457765.427834331,
            "unit": "iter/sec",
            "range": "stddev: 8.570023143785958e-9",
            "extra": "mean: 183.2251703050494 nsec\nrounds: 54813"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5526785.266375492,
            "unit": "iter/sec",
            "range": "stddev: 7.814397801463629e-9",
            "extra": "mean: 180.93700981726914 nsec\nrounds: 55826"
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
          "id": "0209a95a24ff85505f46fe4b2038518b930485d7",
          "message": "Refactor left-shift to speed up cast",
          "timestamp": "2024-03-20T19:59:06+01:00",
          "tree_id": "72307b75fd30328d7199e24cae5e9c1ee3752721",
          "url": "https://github.com/apytypes/apytypes/commit/0209a95a24ff85505f46fe4b2038518b930485d7"
        },
        "date": 1710961392593,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4121784.264727638,
            "unit": "iter/sec",
            "range": "stddev: 1.4420414479227641e-8",
            "extra": "mean: 242.61337706522846 nsec\nrounds: 46188"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2649512.1560112573,
            "unit": "iter/sec",
            "range": "stddev: 3.091197558260419e-8",
            "extra": "mean: 377.42797206295586 nsec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2919119.6812236332,
            "unit": "iter/sec",
            "range": "stddev: 2.4032006633431075e-8",
            "extra": "mean: 342.56903080480805 nsec\nrounds: 193799"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3054393.401226957,
            "unit": "iter/sec",
            "range": "stddev: 2.446318528083172e-8",
            "extra": "mean: 327.3972500065963 nsec\nrounds: 191205"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2383057.5689175352,
            "unit": "iter/sec",
            "range": "stddev: 3.108005091721461e-8",
            "extra": "mean: 419.6289728973057 nsec\nrounds: 182482"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2781925.8529263423,
            "unit": "iter/sec",
            "range": "stddev: 2.703548393236374e-8",
            "extra": "mean: 359.46321105145654 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4497355.386404776,
            "unit": "iter/sec",
            "range": "stddev: 1.0702519545313095e-8",
            "extra": "mean: 222.35289722107416 nsec\nrounds: 45661"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4273029.726931272,
            "unit": "iter/sec",
            "range": "stddev: 1.8981440451532275e-8",
            "extra": "mean: 234.02598715794636 nsec\nrounds: 191976"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3609430.6179707916,
            "unit": "iter/sec",
            "range": "stddev: 2.201792811659425e-8",
            "extra": "mean: 277.05200787648636 nsec\nrounds: 187935"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3531558.6062324736,
            "unit": "iter/sec",
            "range": "stddev: 2.2202589911167974e-8",
            "extra": "mean: 283.1610944345535 nsec\nrounds: 197278"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5576566.790671139,
            "unit": "iter/sec",
            "range": "stddev: 9.526337477134619e-9",
            "extra": "mean: 179.3218009461976 nsec\nrounds: 55854"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 5062805.795685297,
            "unit": "iter/sec",
            "range": "stddev: 1.0913593695632015e-8",
            "extra": "mean: 197.51893324690653 nsec\nrounds: 50977"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8105163.39897953,
            "unit": "iter/sec",
            "range": "stddev: 6.34230458462305e-9",
            "extra": "mean: 123.37814190476601 nsec\nrounds: 80561"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 5059873.690547803,
            "unit": "iter/sec",
            "range": "stddev: 2.3128853168964694e-8",
            "extra": "mean: 197.63339189040508 nsec\nrounds: 52313"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4711233.608656543,
            "unit": "iter/sec",
            "range": "stddev: 1.0325961890547154e-8",
            "extra": "mean: 212.25863182894614 nsec\nrounds: 47304"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5324576.921624032,
            "unit": "iter/sec",
            "range": "stddev: 1.0056576376365444e-8",
            "extra": "mean: 187.8083488546923 nsec\nrounds: 55513"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5397112.058034968,
            "unit": "iter/sec",
            "range": "stddev: 1.050047239863875e-8",
            "extra": "mean: 185.28427596967964 nsec\nrounds: 56745"
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
          "id": "8ace95e9cfab1bebcb2e3ed7617d2f1c093d3ed2",
          "message": "Improve relative time plot",
          "timestamp": "2024-03-21T08:57:19+01:00",
          "tree_id": "57282aa4b07e4c18bfb69a65895ccda11a94ee88",
          "url": "https://github.com/apytypes/apytypes/commit/8ace95e9cfab1bebcb2e3ed7617d2f1c093d3ed2"
        },
        "date": 1711008169097,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4507193.9058187995,
            "unit": "iter/sec",
            "range": "stddev: 1.0237698598882526e-8",
            "extra": "mean: 221.86753463364323 nsec\nrounds: 45474"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2579689.075067635,
            "unit": "iter/sec",
            "range": "stddev: 3.90384449063291e-8",
            "extra": "mean: 387.643615529108 nsec\nrounds: 196890"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3099547.3864092617,
            "unit": "iter/sec",
            "range": "stddev: 2.819834968007176e-8",
            "extra": "mean: 322.62775022725873 nsec\nrounds: 191939"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3028275.605924997,
            "unit": "iter/sec",
            "range": "stddev: 2.9900761402139894e-8",
            "extra": "mean: 330.2209343309844 nsec\nrounds: 197239"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2319392.6753329053,
            "unit": "iter/sec",
            "range": "stddev: 3.6836644204153906e-8",
            "extra": "mean: 431.1473475945472 nsec\nrounds: 189394"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2796322.797557829,
            "unit": "iter/sec",
            "range": "stddev: 3.447650649822357e-8",
            "extra": "mean: 357.61250484869447 nsec\nrounds: 189036"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4248259.778454916,
            "unit": "iter/sec",
            "range": "stddev: 1.6490965066216083e-8",
            "extra": "mean: 235.39050155816074 nsec\nrounds: 43006"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4185060.5207402916,
            "unit": "iter/sec",
            "range": "stddev: 2.1352859613097906e-8",
            "extra": "mean: 238.94517057605816 nsec\nrounds: 192308"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3723171.7121855794,
            "unit": "iter/sec",
            "range": "stddev: 2.523602787555173e-8",
            "extra": "mean: 268.5882030976495 nsec\nrounds: 193462"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3488070.007152078,
            "unit": "iter/sec",
            "range": "stddev: 2.4854853069507195e-8",
            "extra": "mean: 286.69149356213666 nsec\nrounds: 188324"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5231748.070374792,
            "unit": "iter/sec",
            "range": "stddev: 1.1655982123269244e-8",
            "extra": "mean: 191.14070221818673 nsec\nrounds: 52505"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4971147.1199070765,
            "unit": "iter/sec",
            "range": "stddev: 1.3280467055404027e-8",
            "extra": "mean: 201.16081376775128 nsec\nrounds: 49707"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8028233.4478657385,
            "unit": "iter/sec",
            "range": "stddev: 7.557240541271479e-9",
            "extra": "mean: 124.5604037917394 nsec\nrounds: 80685"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4818141.816364312,
            "unit": "iter/sec",
            "range": "stddev: 1.0305843719876327e-8",
            "extra": "mean: 207.54889293703297 nsec\nrounds: 48010"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4704679.980333451,
            "unit": "iter/sec",
            "range": "stddev: 1.3131752586171862e-8",
            "extra": "mean: 212.55430851412908 nsec\nrounds: 46993"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5557030.57042669,
            "unit": "iter/sec",
            "range": "stddev: 1.0886128660800177e-8",
            "extra": "mean: 179.9522222032776 nsec\nrounds: 57731"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5726343.305041943,
            "unit": "iter/sec",
            "range": "stddev: 8.347125325705323e-9",
            "extra": "mean: 174.63151381781483 nsec\nrounds: 56777"
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
          "id": "081a78a10f101f7f997541da8235bf4ddd6aac6b",
          "message": "Improve relative time plot",
          "timestamp": "2024-03-21T10:00:07+01:00",
          "tree_id": "6656184cadd3dc10214fa488fce7605587fbd584",
          "url": "https://github.com/apytypes/apytypes/commit/081a78a10f101f7f997541da8235bf4ddd6aac6b"
        },
        "date": 1711011929931,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4540871.575019201,
            "unit": "iter/sec",
            "range": "stddev: 1.007993356087307e-8",
            "extra": "mean: 220.2220396412566 nsec\nrounds: 46062"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2708918.783050319,
            "unit": "iter/sec",
            "range": "stddev: 3.157976572418122e-8",
            "extra": "mean: 369.15097132370084 nsec\nrounds: 194175"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2856922.5153860524,
            "unit": "iter/sec",
            "range": "stddev: 4.7481907732558444e-8",
            "extra": "mean: 350.0269939469771 nsec\nrounds: 193799"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3032963.5884012515,
            "unit": "iter/sec",
            "range": "stddev: 2.670613080386565e-8",
            "extra": "mean: 329.71051938261644 nsec\nrounds: 199641"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2354916.668358705,
            "unit": "iter/sec",
            "range": "stddev: 3.9423929269725535e-8",
            "extra": "mean: 424.64347610947135 nsec\nrounds: 196851"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2585173.8417004677,
            "unit": "iter/sec",
            "range": "stddev: 3.292457492884319e-8",
            "extra": "mean: 386.82118156601547 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4445994.735497559,
            "unit": "iter/sec",
            "range": "stddev: 1.4044673967672307e-8",
            "extra": "mean: 224.92154388216815 nsec\nrounds: 44699"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4198150.454779385,
            "unit": "iter/sec",
            "range": "stddev: 2.299795389178693e-8",
            "extra": "mean: 238.20013379031576 nsec\nrounds: 199601"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3715644.3705942165,
            "unit": "iter/sec",
            "range": "stddev: 2.36846469701346e-8",
            "extra": "mean: 269.1323227577655 nsec\nrounds: 194553"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3424562.8570995633,
            "unit": "iter/sec",
            "range": "stddev: 2.55874081599134e-8",
            "extra": "mean: 292.0080727755849 nsec\nrounds: 195734"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5435579.196277351,
            "unit": "iter/sec",
            "range": "stddev: 1.3250415232875489e-8",
            "extra": "mean: 183.97303468318 nsec\nrounds: 54780"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4990907.083500886,
            "unit": "iter/sec",
            "range": "stddev: 8.569454421257635e-9",
            "extra": "mean: 200.36437931406698 nsec\nrounds: 50259"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8122069.418687951,
            "unit": "iter/sec",
            "range": "stddev: 8.181467806895949e-9",
            "extra": "mean: 123.12133133208971 nsec\nrounds: 81948"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 5073205.323764474,
            "unit": "iter/sec",
            "range": "stddev: 9.380392569023674e-9",
            "extra": "mean: 197.11404056830506 nsec\nrounds: 51718"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4481130.969281743,
            "unit": "iter/sec",
            "range": "stddev: 1.2422821344229724e-8",
            "extra": "mean: 223.15794982448327 nsec\nrounds: 45640"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 5467905.0069219535,
            "unit": "iter/sec",
            "range": "stddev: 8.683667861840385e-9",
            "extra": "mean: 182.88540103277134 nsec\nrounds: 55482"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5158223.708315064,
            "unit": "iter/sec",
            "range": "stddev: 1.0996439948632993e-8",
            "extra": "mean: 193.86518626325474 nsec\nrounds: 52560"
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
          "distinct": true,
          "id": "5fb5aee480e9440149e548ca399b3aa061218a04",
          "message": "src: fix APyFixed.__div__, closes #198",
          "timestamp": "2024-03-21T11:06:59+01:00",
          "tree_id": "ad81da3d6efed12f76dd5de6fed4d737efe06050",
          "url": "https://github.com/apytypes/apytypes/commit/5fb5aee480e9440149e548ca399b3aa061218a04"
        },
        "date": 1711015980906,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4772208.595803936,
            "unit": "iter/sec",
            "range": "stddev: 1.0253294330533277e-8",
            "extra": "mean: 209.54658203320702 nsec\nrounds: 48406"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2665759.089921221,
            "unit": "iter/sec",
            "range": "stddev: 4.526904901204919e-8",
            "extra": "mean: 375.12767143170817 nsec\nrounds: 191571"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3004654.9447593046,
            "unit": "iter/sec",
            "range": "stddev: 2.8474443851049336e-8",
            "extra": "mean: 332.816918543128 nsec\nrounds: 195734"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3537484.860502854,
            "unit": "iter/sec",
            "range": "stddev: 2.168304590529339e-8",
            "extra": "mean: 282.6867221866021 nsec\nrounds: 198847"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2008838.9773849214,
            "unit": "iter/sec",
            "range": "stddev: 5.22953039589566e-8",
            "extra": "mean: 497.79997862347705 nsec\nrounds: 196464"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2457997.5741769997,
            "unit": "iter/sec",
            "range": "stddev: 2.9032667043941266e-8",
            "extra": "mean: 406.8352265705056 nsec\nrounds: 187970"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4855165.8536372315,
            "unit": "iter/sec",
            "range": "stddev: 1.1712008820488209e-8",
            "extra": "mean: 205.96618738593034 nsec\nrounds: 47015"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4882959.691597352,
            "unit": "iter/sec",
            "range": "stddev: 1.233813706111182e-8",
            "extra": "mean: 204.79382652302778 nsec\nrounds: 45851"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3769649.506186315,
            "unit": "iter/sec",
            "range": "stddev: 2.0740587563455747e-8",
            "extra": "mean: 265.2766519430937 nsec\nrounds: 190115"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3473015.9128505485,
            "unit": "iter/sec",
            "range": "stddev: 2.2220051417920914e-8",
            "extra": "mean: 287.9341831690494 nsec\nrounds: 197668"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5720966.721441484,
            "unit": "iter/sec",
            "range": "stddev: 1.227224826366569e-8",
            "extra": "mean: 174.7956330967953 nsec\nrounds: 57661"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4027549.002933915,
            "unit": "iter/sec",
            "range": "stddev: 1.9871341952063513e-8",
            "extra": "mean: 248.2899647580609 nsec\nrounds: 193424"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 9226130.679070309,
            "unit": "iter/sec",
            "range": "stddev: 6.080084686467974e-9",
            "extra": "mean: 108.38779926114317 nsec\nrounds: 91492"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4950466.377769898,
            "unit": "iter/sec",
            "range": "stddev: 1.0786834787867854e-8",
            "extra": "mean: 202.00116992822308 nsec\nrounds: 48738"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 3880041.8328044093,
            "unit": "iter/sec",
            "range": "stddev: 2.0614322230705816e-8",
            "extra": "mean: 257.72918001702834 nsec\nrounds: 197239"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 7327067.974279675,
            "unit": "iter/sec",
            "range": "stddev: 9.814015171637599e-9",
            "extra": "mean: 136.48024059692105 nsec\nrounds: 73234"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5401890.493112746,
            "unit": "iter/sec",
            "range": "stddev: 8.53117580728337e-9",
            "extra": "mean: 185.1203761488569 nsec\nrounds: 54101"
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
          "id": "6550e822faa3b418af34707dae8dae7edaab1107",
          "message": "Specialize conversion of numpy arrays to APyFloatArray",
          "timestamp": "2024-03-21T12:11:40+01:00",
          "tree_id": "af97207994a9f565b7b75c15f425f71655c29f1b",
          "url": "https://github.com/apytypes/apytypes/commit/6550e822faa3b418af34707dae8dae7edaab1107"
        },
        "date": 1711019741778,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4774263.057515795,
            "unit": "iter/sec",
            "range": "stddev: 2.0364311401575894e-8",
            "extra": "mean: 209.45640991146107 nsec\nrounds: 163372"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2573240.380705138,
            "unit": "iter/sec",
            "range": "stddev: 3.2065805298194665e-8",
            "extra": "mean: 388.61507362402017 nsec\nrounds: 193424"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2886317.9718044773,
            "unit": "iter/sec",
            "range": "stddev: 2.6709227924272986e-8",
            "extra": "mean: 346.46217421939343 nsec\nrounds: 192308"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3505762.196957856,
            "unit": "iter/sec",
            "range": "stddev: 2.1405850245182734e-8",
            "extra": "mean: 285.2446754282987 nsec\nrounds: 187266"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2080574.5438947403,
            "unit": "iter/sec",
            "range": "stddev: 4.24274233253513e-8",
            "extra": "mean: 480.63646790950855 nsec\nrounds: 188324"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2523856.0588085535,
            "unit": "iter/sec",
            "range": "stddev: 5.5571332885302606e-8",
            "extra": "mean: 396.21910944959 nsec\nrounds: 190840"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4622129.779100141,
            "unit": "iter/sec",
            "range": "stddev: 4.83057330076201e-8",
            "extra": "mean: 216.3504807938823 nsec\nrounds: 46839"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4807158.714235482,
            "unit": "iter/sec",
            "range": "stddev: 1.1893237573186441e-8",
            "extra": "mean: 208.02308794982358 nsec\nrounds: 47015"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3737884.248244614,
            "unit": "iter/sec",
            "range": "stddev: 2.1247820350498652e-8",
            "extra": "mean: 267.5310238588754 nsec\nrounds: 194213"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3532105.410830558,
            "unit": "iter/sec",
            "range": "stddev: 4.5259174293218717e-8",
            "extra": "mean: 283.1172583165305 nsec\nrounds: 193837"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 6273769.209248386,
            "unit": "iter/sec",
            "range": "stddev: 1.0285536599826054e-8",
            "extra": "mean: 159.3938136145886 nsec\nrounds: 62977"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4430913.653934425,
            "unit": "iter/sec",
            "range": "stddev: 2.334577636589414e-8",
            "extra": "mean: 225.68708805959173 nsec\nrounds: 43720"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 9229108.302847521,
            "unit": "iter/sec",
            "range": "stddev: 6.218731874348176e-9",
            "extra": "mean: 108.35282967603536 nsec\nrounds: 92166"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 5178245.0023532845,
            "unit": "iter/sec",
            "range": "stddev: 1.0892751897986224e-8",
            "extra": "mean: 193.11562113136824 nsec\nrounds: 52340"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4209522.279789543,
            "unit": "iter/sec",
            "range": "stddev: 2.1242914068070327e-8",
            "extra": "mean: 237.55664741365774 nsec\nrounds: 195313"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 7436892.0544681,
            "unit": "iter/sec",
            "range": "stddev: 8.474897122159049e-9",
            "extra": "mean: 134.4647727404897 nsec\nrounds: 73660"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5565657.456452568,
            "unit": "iter/sec",
            "range": "stddev: 1.0308725970488586e-8",
            "extra": "mean: 179.67329247697685 nsec\nrounds: 55885"
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
          "id": "f02a6dea59557a08d0a5fec7b472e2941500e18b",
          "message": "Speed-up array conversion for APyFloat",
          "timestamp": "2024-03-21T14:16:09+01:00",
          "tree_id": "ac6b39dffe2c2b3c5beadd22b11b7c1fc96fdde7",
          "url": "https://github.com/apytypes/apytypes/commit/f02a6dea59557a08d0a5fec7b472e2941500e18b"
        },
        "date": 1711027247911,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4296551.954682216,
            "unit": "iter/sec",
            "range": "stddev: 1.3077089957141816e-8",
            "extra": "mean: 232.74477081798855 nsec\nrounds: 44244"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2819987.9859025814,
            "unit": "iter/sec",
            "range": "stddev: 2.6314312106152482e-8",
            "extra": "mean: 354.61143983559924 nsec\nrounds: 193799"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3107226.6048334944,
            "unit": "iter/sec",
            "range": "stddev: 2.3897773581667772e-8",
            "extra": "mean: 321.8304060748046 nsec\nrounds: 189754"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3542986.9508738765,
            "unit": "iter/sec",
            "range": "stddev: 1.8507510916299052e-8",
            "extra": "mean: 282.2477231402494 nsec\nrounds: 194932"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2050356.3627162196,
            "unit": "iter/sec",
            "range": "stddev: 3.411752035680108e-8",
            "extra": "mean: 487.72009499609226 nsec\nrounds: 183117"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2490718.3936194577,
            "unit": "iter/sec",
            "range": "stddev: 2.704852525672686e-8",
            "extra": "mean: 401.49059105261483 nsec\nrounds: 189394"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4484638.483055414,
            "unit": "iter/sec",
            "range": "stddev: 1.0711080792396101e-8",
            "extra": "mean: 222.98341410976863 nsec\nrounds: 45227"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4446802.547072369,
            "unit": "iter/sec",
            "range": "stddev: 8.307277660638287e-9",
            "extra": "mean: 224.88068436014245 nsec\nrounds: 44801"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3593986.5443933974,
            "unit": "iter/sec",
            "range": "stddev: 2.1741177283583494e-8",
            "extra": "mean: 278.24255534870736 nsec\nrounds: 196890"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3303085.293368611,
            "unit": "iter/sec",
            "range": "stddev: 2.504895227838186e-8",
            "extra": "mean: 302.747253305195 nsec\nrounds: 193462"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 5686532.027645751,
            "unit": "iter/sec",
            "range": "stddev: 7.822999118030726e-9",
            "extra": "mean: 175.8541049515531 nsec\nrounds: 56712"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4014058.9434346403,
            "unit": "iter/sec",
            "range": "stddev: 3.588476111204718e-8",
            "extra": "mean: 249.12439356069245 nsec\nrounds: 194932"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 9614430.785377072,
            "unit": "iter/sec",
            "range": "stddev: 6.984448864440614e-9",
            "extra": "mean: 104.01031764887988 nsec\nrounds: 95420"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 4999563.956369512,
            "unit": "iter/sec",
            "range": "stddev: 7.573775401381809e-9",
            "extra": "mean: 200.0174432664043 nsec\nrounds: 53009"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4299383.452859593,
            "unit": "iter/sec",
            "range": "stddev: 8.822745660268119e-9",
            "extra": "mean: 232.59148921341009 nsec\nrounds: 42875"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 6695696.134893252,
            "unit": "iter/sec",
            "range": "stddev: 9.594463446630885e-9",
            "extra": "mean: 149.3496687803146 nsec\nrounds: 65968"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 4810154.559883665,
            "unit": "iter/sec",
            "range": "stddev: 8.843220772727908e-9",
            "extra": "mean: 207.89352765086605 nsec\nrounds: 48786"
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
          "id": "bc76fdc0180387ded747b632f4283b7a7bf1e03d",
          "message": "Remove dead code",
          "timestamp": "2024-03-21T14:26:40+01:00",
          "tree_id": "49d0b8724c1964f2c690cc41a2a0cb7d29ca508b",
          "url": "https://github.com/apytypes/apytypes/commit/bc76fdc0180387ded747b632f4283b7a7bf1e03d"
        },
        "date": 1711027862661,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4844057.0048215585,
            "unit": "iter/sec",
            "range": "stddev: 9.052108386342066e-9",
            "extra": "mean: 206.43852849060985 nsec\nrounds: 47645"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2606620.0225417824,
            "unit": "iter/sec",
            "range": "stddev: 3.108237708606281e-8",
            "extra": "mean: 383.6385784471922 nsec\nrounds: 198060"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2892718.1289568353,
            "unit": "iter/sec",
            "range": "stddev: 2.9062342290610432e-8",
            "extra": "mean: 345.6956244681336 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 3430219.0205025417,
            "unit": "iter/sec",
            "range": "stddev: 2.4061344861689813e-8",
            "extra": "mean: 291.5265742576504 nsec\nrounds: 198020"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2055642.459793383,
            "unit": "iter/sec",
            "range": "stddev: 4.1375300292289174e-8",
            "extra": "mean: 486.46591980809393 nsec\nrounds: 186568"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2369565.407520507,
            "unit": "iter/sec",
            "range": "stddev: 4.0562922901169224e-8",
            "extra": "mean: 422.01831476194184 nsec\nrounds: 198847"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4922550.511902605,
            "unit": "iter/sec",
            "range": "stddev: 9.269623650198647e-9",
            "extra": "mean: 203.14672192432715 nsec\nrounds: 48620"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4610566.038942752,
            "unit": "iter/sec",
            "range": "stddev: 1.1412789914643177e-8",
            "extra": "mean: 216.89310847162193 nsec\nrounds: 45598"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3659946.7309679356,
            "unit": "iter/sec",
            "range": "stddev: 2.0615807678567146e-8",
            "extra": "mean: 273.2280203803253 nsec\nrounds: 193088"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3450204.4326901007,
            "unit": "iter/sec",
            "range": "stddev: 2.5400600544115617e-8",
            "extra": "mean: 289.83789787207553 nsec\nrounds: 188324"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 6657463.461278059,
            "unit": "iter/sec",
            "range": "stddev: 6.9865671401428875e-9",
            "extra": "mean: 150.20735837549137 nsec\nrounds: 64396"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4869037.6694086455,
            "unit": "iter/sec",
            "range": "stddev: 1.1709924438489433e-8",
            "extra": "mean: 205.37939278696837 nsec\nrounds: 48879"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 8718895.716384351,
            "unit": "iter/sec",
            "range": "stddev: 7.317558640479704e-9",
            "extra": "mean: 114.69342363166048 nsec\nrounds: 87635"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 5769406.745045521,
            "unit": "iter/sec",
            "range": "stddev: 8.682373535495774e-9",
            "extra": "mean: 173.32804639900348 nsec\nrounds: 56552"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4295561.023131931,
            "unit": "iter/sec",
            "range": "stddev: 1.1568647485153294e-8",
            "extra": "mean: 232.79846209030723 nsec\nrounds: 43117"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 7762187.192463275,
            "unit": "iter/sec",
            "range": "stddev: 7.979780601395959e-9",
            "extra": "mean: 128.82966813411335 nsec\nrounds: 77682"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5707093.754434221,
            "unit": "iter/sec",
            "range": "stddev: 1.1129777763327884e-8",
            "extra": "mean: 175.2205313296173 nsec\nrounds: 58269"
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
          "id": "1879554678b31d0089d3f976add183a1b029fa30",
          "message": "src: APyFixed::operator/() specialize for single limb",
          "timestamp": "2024-03-21T14:34:43+01:00",
          "tree_id": "51245ab8c4db8f2314cda6d31af54d00c25044e6",
          "url": "https://github.com/apytypes/apytypes/commit/1879554678b31d0089d3f976add183a1b029fa30"
        },
        "date": 1711028680488,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4798904.897737324,
            "unit": "iter/sec",
            "range": "stddev: 9.737648169589358e-9",
            "extra": "mean: 208.38087465992257 nsec\nrounds: 47104"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2576699.307846133,
            "unit": "iter/sec",
            "range": "stddev: 3.281254728995135e-8",
            "extra": "mean: 388.0934018785781 nsec\nrounds: 194553"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 3086526.10885528,
            "unit": "iter/sec",
            "range": "stddev: 3.0871065266796164e-8",
            "extra": "mean: 323.9888355815258 nsec\nrounds: 192716"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 4542582.418769167,
            "unit": "iter/sec",
            "range": "stddev: 1.1991305370260132e-8",
            "extra": "mean: 220.1390988235995 nsec\nrounds: 44342"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2006845.0962747463,
            "unit": "iter/sec",
            "range": "stddev: 4.3984597632849166e-8",
            "extra": "mean: 498.2945628720356 nsec\nrounds: 196464"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2489767.7587763323,
            "unit": "iter/sec",
            "range": "stddev: 4.190634434806478e-8",
            "extra": "mean: 401.64388685449546 nsec\nrounds: 191939"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4637778.523707077,
            "unit": "iter/sec",
            "range": "stddev: 9.634762798708317e-9",
            "extra": "mean: 215.6204732261672 nsec\nrounds: 46426"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4503442.718171443,
            "unit": "iter/sec",
            "range": "stddev: 1.1548455715532649e-8",
            "extra": "mean: 222.0523414153314 nsec\nrounds: 44960"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3721308.840594616,
            "unit": "iter/sec",
            "range": "stddev: 2.1496297454499474e-8",
            "extra": "mean: 268.72265722524963 nsec\nrounds: 199204"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3558919.3838472795,
            "unit": "iter/sec",
            "range": "stddev: 2.916296067052549e-8",
            "extra": "mean: 280.9841674241002 nsec\nrounds: 198020"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 6158012.588864662,
            "unit": "iter/sec",
            "range": "stddev: 1.066929867764763e-8",
            "extra": "mean: 162.39005451343442 nsec\nrounds: 62186"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4468412.581478146,
            "unit": "iter/sec",
            "range": "stddev: 9.474658015664585e-9",
            "extra": "mean: 223.79312155398998 nsec\nrounds: 44244"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 9415884.005081683,
            "unit": "iter/sec",
            "range": "stddev: 6.379544548648291e-9",
            "extra": "mean: 106.20351731818492 nsec\nrounds: 94518"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 5686602.029588941,
            "unit": "iter/sec",
            "range": "stddev: 1.0995012927013237e-8",
            "extra": "mean: 175.85194019144828 nsec\nrounds: 55979"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4487270.191108437,
            "unit": "iter/sec",
            "range": "stddev: 9.76796128736093e-9",
            "extra": "mean: 222.85263810973845 nsec\nrounds: 45309"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 7081540.851719734,
            "unit": "iter/sec",
            "range": "stddev: 9.624767911450912e-9",
            "extra": "mean: 141.21220521624497 nsec\nrounds: 72333"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5358623.98451657,
            "unit": "iter/sec",
            "range": "stddev: 9.494691756589496e-9",
            "extra": "mean: 186.61507187099966 nsec\nrounds: 55516"
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
          "distinct": true,
          "id": "16fa3b966d54c3882e6d9c770fbe9dad72203278",
          "message": "src: APyFixed.cc, explicit template instantiation",
          "timestamp": "2024-03-21T14:42:13+01:00",
          "tree_id": "7dbac5963ee33f6aa5c65d66c0cb57c87508bfe3",
          "url": "https://github.com/apytypes/apytypes/commit/16fa3b966d54c3882e6d9c770fbe9dad72203278"
        },
        "date": 1711028934995,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_short",
            "value": 4747542.127198968,
            "unit": "iter/sec",
            "range": "stddev: 9.642820989136271e-9",
            "extra": "mean: 210.63530837804376 nsec\nrounds: 46404"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_long",
            "value": 2595257.071057714,
            "unit": "iter/sec",
            "range": "stddev: 3.597059272909132e-8",
            "extra": "mean: 385.31828355344675 nsec\nrounds: 199243"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_multiplication_mixed",
            "value": 2947453.3388752015,
            "unit": "iter/sec",
            "range": "stddev: 2.546635469239208e-8",
            "extra": "mean: 339.27593926965227 nsec\nrounds: 196464"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_short",
            "value": 4947712.466708859,
            "unit": "iter/sec",
            "range": "stddev: 1.0576817441989848e-8",
            "extra": "mean: 202.11360436329096 nsec\nrounds: 47215"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_long",
            "value": 2122380.788409465,
            "unit": "iter/sec",
            "range": "stddev: 3.921567140450671e-8",
            "extra": "mean: 471.1689841244107 nsec\nrounds: 190877"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_division_mixed",
            "value": 2700427.833587121,
            "unit": "iter/sec",
            "range": "stddev: 3.004459763276109e-8",
            "extra": "mean: 370.3116919335206 nsec\nrounds: 188324"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short",
            "value": 4610933.346171735,
            "unit": "iter/sec",
            "range": "stddev: 1.1978557865251711e-8",
            "extra": "mean: 216.87583075351984 nsec\nrounds: 46169"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_short_different_wl",
            "value": 4391508.222154755,
            "unit": "iter/sec",
            "range": "stddev: 1.7868946216721068e-8",
            "extra": "mean: 227.71220032233424 nsec\nrounds: 195351"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_long",
            "value": 3689177.2956181904,
            "unit": "iter/sec",
            "range": "stddev: 2.3158661322077684e-8",
            "extra": "mean: 271.0631449423018 nsec\nrounds: 194970"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_addition_mixed",
            "value": 3552364.6308356267,
            "unit": "iter/sec",
            "range": "stddev: 2.2777853489020347e-8",
            "extra": "mean: 281.5026338568913 nsec\nrounds: 196079"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_short",
            "value": 6067978.5638480345,
            "unit": "iter/sec",
            "range": "stddev: 8.72201179777404e-9",
            "extra": "mean: 164.7995274666677 nsec\nrounds: 60165"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_creation_long",
            "value": 4340369.497604518,
            "unit": "iter/sec",
            "range": "stddev: 1.7039300871134013e-8",
            "extra": "mean: 230.39513123292122 nsec\nrounds: 193424"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_to_float_short",
            "value": 9338968.711136837,
            "unit": "iter/sec",
            "range": "stddev: 8.163162319469953e-9",
            "extra": "mean: 107.07820434255525 nsec\nrounds: 91660"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_short",
            "value": 5656005.836898193,
            "unit": "iter/sec",
            "range": "stddev: 8.16379494265578e-9",
            "extra": "mean: 176.80321216716345 nsec\nrounds: 54994"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_negation_long",
            "value": 4477658.285007552,
            "unit": "iter/sec",
            "range": "stddev: 9.518927271288466e-9",
            "extra": "mean: 223.33102178612404 nsec\nrounds: 44481"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_short",
            "value": 7176813.230102342,
            "unit": "iter/sec",
            "range": "stddev: 7.351762276029925e-9",
            "extra": "mean: 139.3376095960344 nsec\nrounds: 71649"
          },
          {
            "name": "benchmark/ci_fixed_scalar_benchmarks.py::test_fixed_scalar_abs_long",
            "value": 5358427.3468622975,
            "unit": "iter/sec",
            "range": "stddev: 8.952065135075173e-9",
            "extra": "mean: 186.62192006498307 nsec\nrounds: 53868"
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
        "date": 1710599426257,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2188223.082109382,
            "unit": "iter/sec",
            "range": "stddev: 3.9882033363921313e-8",
            "extra": "mean: 456.9917976718219 nsec\nrounds: 197239"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1620508.5640409219,
            "unit": "iter/sec",
            "range": "stddev: 6.350240307225162e-8",
            "extra": "mean: 617.0902284566794 nsec\nrounds: 165235"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1743765.4435953149,
            "unit": "iter/sec",
            "range": "stddev: 5.053343036661495e-8",
            "extra": "mean: 573.4716235333751 nsec\nrounds: 172118"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1348522.3075966686,
            "unit": "iter/sec",
            "range": "stddev: 6.299805862000562e-8",
            "extra": "mean: 741.5524343695063 nsec\nrounds: 133977"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1291529.6425054332,
            "unit": "iter/sec",
            "range": "stddev: 5.824771831590884e-8",
            "extra": "mean: 774.2756860462118 nsec\nrounds: 128453"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6522548.166099454,
            "unit": "iter/sec",
            "range": "stddev: 8.494934943824896e-9",
            "extra": "mean: 153.31431436528285 nsec\nrounds: 65669"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6628178.918114183,
            "unit": "iter/sec",
            "range": "stddev: 9.778163139676385e-9",
            "extra": "mean: 150.87100278283614 nsec\nrounds: 67173"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10670730.125171352,
            "unit": "iter/sec",
            "range": "stddev: 1.3489227792842746e-8",
            "extra": "mean: 93.71429960919926 nsec\nrounds: 103542"
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
          "id": "1085f94a22bc3a38032b3bf5229073b8456b1983",
          "message": "More fixed-point array benchmarks",
          "timestamp": "2024-03-16T15:46:49+01:00",
          "tree_id": "eab1295361478d03a914832f57e36855cbc57364",
          "url": "https://github.com/apytypes/apytypes/commit/1085f94a22bc3a38032b3bf5229073b8456b1983"
        },
        "date": 1710600669936,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2316151.5559991095,
            "unit": "iter/sec",
            "range": "stddev: 3.9303592450895035e-8",
            "extra": "mean: 431.75067599090227 nsec\nrounds: 193051"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1660348.6717279793,
            "unit": "iter/sec",
            "range": "stddev: 4.7175496753592504e-8",
            "extra": "mean: 602.2831330718185 nsec\nrounds: 167197"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1306729.7806285562,
            "unit": "iter/sec",
            "range": "stddev: 6.566489812929456e-8",
            "extra": "mean: 765.2691587994733 nsec\nrounds: 131493"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1544689.2331596562,
            "unit": "iter/sec",
            "range": "stddev: 5.227161742844389e-8",
            "extra": "mean: 647.3794071539844 nsec\nrounds: 151470"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1619001.2474444536,
            "unit": "iter/sec",
            "range": "stddev: 4.985695517696729e-8",
            "extra": "mean: 617.6647495352412 nsec\nrounds: 159439"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6575092.404973492,
            "unit": "iter/sec",
            "range": "stddev: 8.937741633465449e-9",
            "extra": "mean: 152.08911729417616 nsec\nrounds: 65322"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6608148.758985072,
            "unit": "iter/sec",
            "range": "stddev: 7.245710144550509e-9",
            "extra": "mean: 151.328312432488 nsec\nrounds: 66587"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10702780.402495861,
            "unit": "iter/sec",
            "range": "stddev: 8.886571022501871e-9",
            "extra": "mean: 93.4336651218522 nsec\nrounds: 103972"
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
          "id": "3cdd779f89b4253f80f05a35d5f25be8fb8e9dca",
          "message": "Compute shift amount less often",
          "timestamp": "2024-03-16T18:19:28+01:00",
          "tree_id": "181c28f8026c39ffd5cf445e4725d749df290e55",
          "url": "https://github.com/apytypes/apytypes/commit/3cdd779f89b4253f80f05a35d5f25be8fb8e9dca"
        },
        "date": 1710609826959,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2113358.2806106517,
            "unit": "iter/sec",
            "range": "stddev: 4.182517100711691e-8",
            "extra": "mean: 473.18053411702704 nsec\nrounds: 188324"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1550095.1327866735,
            "unit": "iter/sec",
            "range": "stddev: 5.039700861444662e-8",
            "extra": "mean: 645.12169533895 nsec\nrounds: 153563"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1539573.0080481821,
            "unit": "iter/sec",
            "range": "stddev: 6.104851645180958e-8",
            "extra": "mean: 649.5307431167261 nsec\nrounds: 147864"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1071519.328755014,
            "unit": "iter/sec",
            "range": "stddev: 7.366254860668494e-8",
            "extra": "mean: 933.254280314201 nsec\nrounds: 103649"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1303864.942083747,
            "unit": "iter/sec",
            "range": "stddev: 7.015628291265082e-8",
            "extra": "mean: 766.9506002683685 nsec\nrounds: 131857"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5839591.107446084,
            "unit": "iter/sec",
            "range": "stddev: 1.1388225559604431e-8",
            "extra": "mean: 171.24486656691562 nsec\nrounds: 56358"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5774162.737807021,
            "unit": "iter/sec",
            "range": "stddev: 1.019878568928451e-8",
            "extra": "mean: 173.18528164305508 nsec\nrounds: 54040"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9695948.58849589,
            "unit": "iter/sec",
            "range": "stddev: 6.8466146689274645e-9",
            "extra": "mean: 103.13586039294347 nsec\nrounds: 95067"
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
          "id": "a78d22c8c9593f9c803cb11afde930b4e65a5d70",
          "message": "Inline scalar cast in array",
          "timestamp": "2024-03-16T19:17:31+01:00",
          "tree_id": "eeb97c983d61e9e692a2ea000ecf242953c9313e",
          "url": "https://github.com/apytypes/apytypes/commit/a78d22c8c9593f9c803cb11afde930b4e65a5d70"
        },
        "date": 1710613353128,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2273810.8833306646,
            "unit": "iter/sec",
            "range": "stddev: 3.3248029490724574e-8",
            "extra": "mean: 439.7903129635826 nsec\nrounds: 188680"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1708332.310360961,
            "unit": "iter/sec",
            "range": "stddev: 4.5859451037042136e-8",
            "extra": "mean: 585.3662041834529 nsec\nrounds: 166639"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1720353.5410732753,
            "unit": "iter/sec",
            "range": "stddev: 4.341204014819542e-8",
            "extra": "mean: 581.2758692472405 nsec\nrounds: 173281"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 911140.821603481,
            "unit": "iter/sec",
            "range": "stddev: 6.272789804985262e-8",
            "extra": "mean: 1.0975251863264528 usec\nrounds: 89358"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1203609.0683779977,
            "unit": "iter/sec",
            "range": "stddev: 6.094385369523115e-8",
            "extra": "mean: 830.8345510786233 nsec\nrounds: 119675"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6820913.834025305,
            "unit": "iter/sec",
            "range": "stddev: 7.950701053952343e-9",
            "extra": "mean: 146.60792150926574 nsec\nrounds: 64898"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6813620.6694703195,
            "unit": "iter/sec",
            "range": "stddev: 9.467562796389665e-9",
            "extra": "mean: 146.76484772348002 nsec\nrounds: 68461"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9605083.61331,
            "unit": "iter/sec",
            "range": "stddev: 5.829778139171972e-9",
            "extra": "mean: 104.11153512652264 nsec\nrounds: 95979"
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
          "id": "0e8326562c018e28bef8d0facf1f1b0b82128c20",
          "message": "Remove unused arguments",
          "timestamp": "2024-03-16T19:30:41+01:00",
          "tree_id": "3eb7bbd376c47582af0b56edcd3bd0a27378db8e",
          "url": "https://github.com/apytypes/apytypes/commit/0e8326562c018e28bef8d0facf1f1b0b82128c20"
        },
        "date": 1710614083371,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2228826.059449793,
            "unit": "iter/sec",
            "range": "stddev: 3.691225458598555e-8",
            "extra": "mean: 448.6666852084045 nsec\nrounds: 191205"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1594224.953596548,
            "unit": "iter/sec",
            "range": "stddev: 5.415135818148085e-8",
            "extra": "mean: 627.26404937025 nsec\nrounds: 159185"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1607927.0107734979,
            "unit": "iter/sec",
            "range": "stddev: 8.158588535981379e-8",
            "extra": "mean: 621.9187769716887 nsec\nrounds: 158705"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1311197.3246801058,
            "unit": "iter/sec",
            "range": "stddev: 4.755876840771238e-8",
            "extra": "mean: 762.6617147378506 nsec\nrounds: 130481"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1177482.8252712297,
            "unit": "iter/sec",
            "range": "stddev: 5.96327412806209e-8",
            "extra": "mean: 849.269287447626 nsec\nrounds: 117014"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6784237.857218063,
            "unit": "iter/sec",
            "range": "stddev: 6.831996335323367e-9",
            "extra": "mean: 147.40049229496617 nsec\nrounds: 68597"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6855001.843144427,
            "unit": "iter/sec",
            "range": "stddev: 1.1102086462016621e-8",
            "extra": "mean: 145.878881272725 nsec\nrounds: 68274"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10288625.581493173,
            "unit": "iter/sec",
            "range": "stddev: 5.451732377248311e-9",
            "extra": "mean: 97.1947119738699 nsec\nrounds: 103445"
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
          "id": "7bbc14782cc3e9012e2c2a2fc782850da70caed2",
          "message": "Add test",
          "timestamp": "2024-03-16T20:08:08+01:00",
          "tree_id": "9e2df6f83d929296daf4e20e9b411e0019de64cd",
          "url": "https://github.com/apytypes/apytypes/commit/7bbc14782cc3e9012e2c2a2fc782850da70caed2"
        },
        "date": 1710616333832,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2073915.2531000914,
            "unit": "iter/sec",
            "range": "stddev: 4.050888900930747e-8",
            "extra": "mean: 482.1797797693787 nsec\nrounds: 184502"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1640561.3582436906,
            "unit": "iter/sec",
            "range": "stddev: 4.3252063064884794e-8",
            "extra": "mean: 609.5474545801565 nsec\nrounds: 161525"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1526859.905884632,
            "unit": "iter/sec",
            "range": "stddev: 4.9524356187023705e-8",
            "extra": "mean: 654.9389345714877 nsec\nrounds: 80756"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1445701.9917291289,
            "unit": "iter/sec",
            "range": "stddev: 5.949582205227338e-8",
            "extra": "mean: 691.7054868298665 nsec\nrounds: 145075"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1221925.4547035948,
            "unit": "iter/sec",
            "range": "stddev: 5.358549705467272e-8",
            "extra": "mean: 818.3805289845968 nsec\nrounds: 121139"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6277467.569893185,
            "unit": "iter/sec",
            "range": "stddev: 8.585566015399258e-9",
            "extra": "mean: 159.29990698730853 nsec\nrounds: 66765"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6303353.174641261,
            "unit": "iter/sec",
            "range": "stddev: 7.802373860256213e-9",
            "extra": "mean: 158.64571955492306 nsec\nrounds: 67806"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9013083.737018984,
            "unit": "iter/sec",
            "range": "stddev: 6.194491552448258e-9",
            "extra": "mean: 110.94981797323922 nsec\nrounds: 96909"
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
          "id": "cb733a73063b35d56a76fb9fcd535011749fcd08",
          "message": "Add early return for cast",
          "timestamp": "2024-03-16T21:27:41+01:00",
          "tree_id": "ae4553afcd999df438dddc2d3d0ff31514975ddb",
          "url": "https://github.com/apytypes/apytypes/commit/cb733a73063b35d56a76fb9fcd535011749fcd08"
        },
        "date": 1710621169382,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2223447.8937087357,
            "unit": "iter/sec",
            "range": "stddev: 3.639122434967859e-8",
            "extra": "mean: 449.7519383429188 nsec\nrounds: 189036"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1676760.7708164994,
            "unit": "iter/sec",
            "range": "stddev: 4.6072257960948896e-8",
            "extra": "mean: 596.3879984579313 nsec\nrounds: 166362"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1462812.6546687614,
            "unit": "iter/sec",
            "range": "stddev: 6.430656273353996e-8",
            "extra": "mean: 683.6145399811014 nsec\nrounds: 146135"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1617472.6490970536,
            "unit": "iter/sec",
            "range": "stddev: 5.430169421977978e-8",
            "extra": "mean: 618.2484758293789 nsec\nrounds: 159465"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1492760.5906875927,
            "unit": "iter/sec",
            "range": "stddev: 4.9773871436575255e-8",
            "extra": "mean: 669.8997858319266 nsec\nrounds: 149881"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5543536.103377186,
            "unit": "iter/sec",
            "range": "stddev: 1.5297948580601797e-8",
            "extra": "mean: 180.3902746102231 nsec\nrounds: 57827"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5588261.219315404,
            "unit": "iter/sec",
            "range": "stddev: 9.163619813584132e-9",
            "extra": "mean: 178.94653824406276 nsec\nrounds: 53548"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10547054.374307413,
            "unit": "iter/sec",
            "range": "stddev: 5.8756614755970854e-9",
            "extra": "mean: 94.8132022942441 nsec\nrounds: 104625"
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
          "id": "6f11740433df726d742b3675ec23303d2ca5a661",
          "message": "Add early return for cast and no quantization casting for arrays",
          "timestamp": "2024-03-16T23:56:33+01:00",
          "tree_id": "4373ecb5b36dff857ebbb054130efae2a3c81e8a",
          "url": "https://github.com/apytypes/apytypes/commit/6f11740433df726d742b3675ec23303d2ca5a661"
        },
        "date": 1710630049739,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2212582.5701842196,
            "unit": "iter/sec",
            "range": "stddev: 4.204853858341768e-8",
            "extra": "mean: 451.9605340273211 nsec\nrounds: 185529"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1743550.2945661584,
            "unit": "iter/sec",
            "range": "stddev: 5.5788594983688744e-8",
            "extra": "mean: 573.5423882617556 nsec\nrounds: 176648"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1762048.3201376947,
            "unit": "iter/sec",
            "range": "stddev: 4.3501453127658594e-8",
            "extra": "mean: 567.5213264990181 nsec\nrounds: 175439"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1613854.3402669306,
            "unit": "iter/sec",
            "range": "stddev: 1.0870559109607829e-7",
            "extra": "mean: 619.6346070702031 nsec\nrounds: 163640"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1043468.7148719934,
            "unit": "iter/sec",
            "range": "stddev: 6.305207532959805e-8",
            "extra": "mean: 958.3421004842126 nsec\nrounds: 103424"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6238099.75502161,
            "unit": "iter/sec",
            "range": "stddev: 8.24988959934436e-9",
            "extra": "mean: 160.30522743644815 nsec\nrounds: 62193"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6253243.835591377,
            "unit": "iter/sec",
            "range": "stddev: 8.423505999821353e-9",
            "extra": "mean: 159.91700088653727 nsec\nrounds: 63172"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10723961.840234147,
            "unit": "iter/sec",
            "range": "stddev: 6.971774150514166e-9",
            "extra": "mean: 93.2491195788777 nsec\nrounds: 104734"
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
          "id": "794f95feac30dbced9c758d57e8c8bf546703adf",
          "message": "src: add faster APyFixedArray add/sub",
          "timestamp": "2024-03-18T09:40:47+01:00",
          "tree_id": "14f6e7a9f1597c5186a5f0637ca03ad956fabcdd",
          "url": "https://github.com/apytypes/apytypes/commit/794f95feac30dbced9c758d57e8c8bf546703adf"
        },
        "date": 1710751804491,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2271200.689554439,
            "unit": "iter/sec",
            "range": "stddev: 4.315104715748307e-8",
            "extra": "mean: 440.29574515326846 nsec\nrounds: 186916"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1659989.3206229517,
            "unit": "iter/sec",
            "range": "stddev: 5.537588917345774e-8",
            "extra": "mean: 602.4135140969996 nsec\nrounds: 167477"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1464103.4294929816,
            "unit": "iter/sec",
            "range": "stddev: 5.324427960515652e-8",
            "extra": "mean: 683.011855485 nsec\nrounds: 146135"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1220254.8603659335,
            "unit": "iter/sec",
            "range": "stddev: 6.512676071406028e-8",
            "extra": "mean: 819.5009358128057 nsec\nrounds: 118400"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1482395.9403010588,
            "unit": "iter/sec",
            "range": "stddev: 5.888673469997199e-8",
            "extra": "mean: 674.5836067231097 nsec\nrounds: 147646"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5865367.335276467,
            "unit": "iter/sec",
            "range": "stddev: 9.366780667101613e-9",
            "extra": "mean: 170.49230556889344 nsec\nrounds: 58337"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5508235.191467043,
            "unit": "iter/sec",
            "range": "stddev: 1.5200049569108612e-8",
            "extra": "mean: 181.54635109790289 nsec\nrounds: 58371"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10201780.759323226,
            "unit": "iter/sec",
            "range": "stddev: 6.271967093822189e-9",
            "extra": "mean: 98.02210257126498 nsec\nrounds: 101647"
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
          "id": "d18aef6266a3990a9fcfd7bafd748d9aab5b9a81",
          "message": "Faster unary operations (and some binary)",
          "timestamp": "2024-03-18T11:03:44+01:00",
          "tree_id": "43144e49a14d27b7d4f4b682e571c77f3d7f4f91",
          "url": "https://github.com/apytypes/apytypes/commit/d18aef6266a3990a9fcfd7bafd748d9aab5b9a81"
        },
        "date": 1710756497633,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2248803.5132116256,
            "unit": "iter/sec",
            "range": "stddev: 4.0435115284454196e-8",
            "extra": "mean: 444.6809132612271 nsec\nrounds: 186220"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1548471.0759467322,
            "unit": "iter/sec",
            "range": "stddev: 1.2182136381973036e-7",
            "extra": "mean: 645.7983074618534 nsec\nrounds: 157928"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1559875.7040850108,
            "unit": "iter/sec",
            "range": "stddev: 5.844891656414122e-8",
            "extra": "mean: 641.07672001125 nsec\nrounds: 155232"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1488406.2630849008,
            "unit": "iter/sec",
            "range": "stddev: 5.4788359426559895e-8",
            "extra": "mean: 671.8595754411099 nsec\nrounds: 148766"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1199756.9601956243,
            "unit": "iter/sec",
            "range": "stddev: 6.048849433048975e-8",
            "extra": "mean: 833.5021451652453 nsec\nrounds: 119105"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5402652.284425528,
            "unit": "iter/sec",
            "range": "stddev: 9.981746117619207e-9",
            "extra": "mean: 185.09427358162424 nsec\nrounds: 53695"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5391433.498901614,
            "unit": "iter/sec",
            "range": "stddev: 9.15080923270655e-9",
            "extra": "mean: 185.4794277261573 nsec\nrounds: 53663"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10030240.406159937,
            "unit": "iter/sec",
            "range": "stddev: 7.282472468232606e-9",
            "extra": "mean: 99.6985076634519 nsec\nrounds: 100614"
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
          "id": "8624f1a830a788c51c318cf00d5e3a9111d50647",
          "message": "Use inline shift for APyFixed",
          "timestamp": "2024-03-18T12:24:47+01:00",
          "tree_id": "ce60e8d72d0599048f0325a173b54ba40c955da0",
          "url": "https://github.com/apytypes/apytypes/commit/8624f1a830a788c51c318cf00d5e3a9111d50647"
        },
        "date": 1710761344684,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2365537.49774281,
            "unit": "iter/sec",
            "range": "stddev: 3.361087867945921e-8",
            "extra": "mean: 422.7369048067052 nsec\nrounds: 190477"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1750675.1805809091,
            "unit": "iter/sec",
            "range": "stddev: 4.504436575453295e-8",
            "extra": "mean: 571.2081893274569 nsec\nrounds: 173311"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1902826.5833446276,
            "unit": "iter/sec",
            "range": "stddev: 4.362178973756875e-8",
            "extra": "mean: 525.5339654977612 nsec\nrounds: 188715"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1719801.5027779478,
            "unit": "iter/sec",
            "range": "stddev: 5.114198326068216e-8",
            "extra": "mean: 581.4624527218916 nsec\nrounds: 165783"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1780142.5346797751,
            "unit": "iter/sec",
            "range": "stddev: 4.5967300776803117e-8",
            "extra": "mean: 561.7527700836074 nsec\nrounds: 177305"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 4938861.739377451,
            "unit": "iter/sec",
            "range": "stddev: 8.873882093191547e-9",
            "extra": "mean: 202.47580369120627 nsec\nrounds: 58337"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5835299.321142455,
            "unit": "iter/sec",
            "range": "stddev: 1.5538630761622455e-8",
            "extra": "mean: 171.3708149257574 nsec\nrounds: 59981"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10486231.390423445,
            "unit": "iter/sec",
            "range": "stddev: 6.1675395152736524e-9",
            "extra": "mean: 95.3631445624416 nsec\nrounds: 104625"
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
          "id": "1cfd03b3ce602906dafa7e688b2d03ef70f9f403",
          "message": "Reuse variables for floating-point array arithmetic",
          "timestamp": "2024-03-18T12:45:51+01:00",
          "tree_id": "a7bcef98986f1516c9269c25df3071bc76bb461b",
          "url": "https://github.com/apytypes/apytypes/commit/1cfd03b3ce602906dafa7e688b2d03ef70f9f403"
        },
        "date": 1710762598085,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 2255050.968499042,
            "unit": "iter/sec",
            "range": "stddev: 3.7091363874311323e-8",
            "extra": "mean: 443.44895701651944 nsec\nrounds: 187618"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1681216.306182943,
            "unit": "iter/sec",
            "range": "stddev: 4.3395070024188285e-8",
            "extra": "mean: 594.8074595294198 nsec\nrounds: 167477"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1668544.6316399209,
            "unit": "iter/sec",
            "range": "stddev: 5.0521754931411036e-8",
            "extra": "mean: 599.3246935307807 nsec\nrounds: 167477"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1483934.0629830386,
            "unit": "iter/sec",
            "range": "stddev: 4.66770831471714e-8",
            "extra": "mean: 673.8843894382732 nsec\nrounds: 145709"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1542921.5547105693,
            "unit": "iter/sec",
            "range": "stddev: 4.503741717713658e-8",
            "extra": "mean: 648.1210901143581 nsec\nrounds: 152626"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5636815.27914921,
            "unit": "iter/sec",
            "range": "stddev: 8.428996179275879e-9",
            "extra": "mean: 177.40513933436654 nsec\nrounds: 56231"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5637038.023093395,
            "unit": "iter/sec",
            "range": "stddev: 7.651202112789852e-9",
            "extra": "mean: 177.39812928400553 nsec\nrounds: 56139"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10247008.77561695,
            "unit": "iter/sec",
            "range": "stddev: 5.8259559168404364e-9",
            "extra": "mean: 97.5894548250309 nsec\nrounds: 101637"
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
          "id": "ce8522b7e8c05ebc181b8b92a8314f805a068c34",
          "message": "Add fast path for floating-point multiplication",
          "timestamp": "2024-03-18T13:53:20+01:00",
          "tree_id": "e43a5b86300e1757e76647e397115aae9b8e8b56",
          "url": "https://github.com/apytypes/apytypes/commit/ce8522b7e8c05ebc181b8b92a8314f805a068c34"
        },
        "date": 1710766657910,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4328506.971000609,
            "unit": "iter/sec",
            "range": "stddev: 1.3475625478809475e-8",
            "extra": "mean: 231.02654256997238 nsec\nrounds: 43703"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1733939.9054746649,
            "unit": "iter/sec",
            "range": "stddev: 4.853795937275715e-8",
            "extra": "mean: 576.7212559343382 nsec\nrounds: 171792"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1885818.0884900305,
            "unit": "iter/sec",
            "range": "stddev: 4.499299222692921e-8",
            "extra": "mean: 530.2738403579209 nsec\nrounds: 188680"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1726195.2143191407,
            "unit": "iter/sec",
            "range": "stddev: 4.5223779238779767e-8",
            "extra": "mean: 579.3087547138248 nsec\nrounds: 169463"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1721652.5365532502,
            "unit": "iter/sec",
            "range": "stddev: 4.683836760169538e-8",
            "extra": "mean: 580.8372936865758 nsec\nrounds: 170069"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5860428.8023176575,
            "unit": "iter/sec",
            "range": "stddev: 7.688331747093297e-9",
            "extra": "mean: 170.63597796878585 nsec\nrounds: 58371"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5449675.339952364,
            "unit": "iter/sec",
            "range": "stddev: 7.67826808261986e-9",
            "extra": "mean: 183.4971695779407 nsec\nrounds: 57797"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10060972.234880764,
            "unit": "iter/sec",
            "range": "stddev: 9.338842684038804e-9",
            "extra": "mean: 99.39397273487556 nsec\nrounds: 101225"
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
          "id": "4c6ca2566921c2fc9b46840887dee88ec93197c4",
          "message": "Add unary negation and specialize array/scalar operations",
          "timestamp": "2024-03-18T17:30:20+01:00",
          "tree_id": "cb4c776d6cce7c51122365014a72eeb43deb5473",
          "url": "https://github.com/apytypes/apytypes/commit/4c6ca2566921c2fc9b46840887dee88ec93197c4"
        },
        "date": 1710779672181,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4081970.398478065,
            "unit": "iter/sec",
            "range": "stddev: 2.070426466590461e-8",
            "extra": "mean: 244.97972850862755 nsec\nrounds: 194970"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1747600.8572185119,
            "unit": "iter/sec",
            "range": "stddev: 4.827716799920802e-8",
            "extra": "mean: 572.2130404488751 nsec\nrounds: 176336"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 2065991.115349836,
            "unit": "iter/sec",
            "range": "stddev: 4.4745157060053016e-8",
            "extra": "mean: 484.0291870426245 nsec\nrounds: 94518"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1892246.116152807,
            "unit": "iter/sec",
            "range": "stddev: 5.348532924383233e-8",
            "extra": "mean: 528.472481176474 nsec\nrounds: 185186"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1669944.6758160414,
            "unit": "iter/sec",
            "range": "stddev: 4.526141094093021e-8",
            "extra": "mean: 598.8222331444807 nsec\nrounds: 164718"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5356593.112886255,
            "unit": "iter/sec",
            "range": "stddev: 8.004835352034174e-9",
            "extra": "mean: 186.68582416584016 nsec\nrounds: 53721"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5316893.8447504025,
            "unit": "iter/sec",
            "range": "stddev: 9.094665606376058e-9",
            "extra": "mean: 188.07973775659417 nsec\nrounds: 52203"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10461808.159274967,
            "unit": "iter/sec",
            "range": "stddev: 5.9584641533827065e-9",
            "extra": "mean: 95.58577109953956 nsec\nrounds: 102062"
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
          "id": "386af56e4182053c65559ed6fbe59e22eff8b257",
          "message": "Special casing for double to fixed-point conversion",
          "timestamp": "2024-03-18T17:48:54+01:00",
          "tree_id": "22585c10515b3eaa1b1e87d7ca88e3d502dda7f9",
          "url": "https://github.com/apytypes/apytypes/commit/386af56e4182053c65559ed6fbe59e22eff8b257"
        },
        "date": 1710780779736,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4453520.174430493,
            "unit": "iter/sec",
            "range": "stddev: 1.035239954231275e-8",
            "extra": "mean: 224.5414774904207 nsec\nrounds: 46254"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1749699.9505608403,
            "unit": "iter/sec",
            "range": "stddev: 4.735156908603906e-8",
            "extra": "mean: 571.5265635570589 nsec\nrounds: 174521"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1569198.9017013563,
            "unit": "iter/sec",
            "range": "stddev: 4.7964707823462895e-8",
            "extra": "mean: 637.267843430065 nsec\nrounds: 157184"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1697554.7744351255,
            "unit": "iter/sec",
            "range": "stddev: 4.773777072624792e-8",
            "extra": "mean: 589.0826116833648 nsec\nrounds: 169177"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1793806.1771273855,
            "unit": "iter/sec",
            "range": "stddev: 4.2878936513648266e-8",
            "extra": "mean: 557.4738300887313 nsec\nrounds: 173883"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5649473.678588046,
            "unit": "iter/sec",
            "range": "stddev: 9.654271674629592e-9",
            "extra": "mean: 177.0076394531798 nsec\nrounds: 56453"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5642940.458913973,
            "unit": "iter/sec",
            "range": "stddev: 9.001011836161952e-9",
            "extra": "mean: 177.21257335262234 nsec\nrounds: 49488"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10043910.905891482,
            "unit": "iter/sec",
            "range": "stddev: 5.27869408443919e-9",
            "extra": "mean: 99.56281067900079 nsec\nrounds: 98542"
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
          "id": "6f1613718018f34beef7cc2801b3f740c395b1f8",
          "message": "src: fix memory out-of-bounds access bug, closes #179",
          "timestamp": "2024-03-18T18:18:25+01:00",
          "tree_id": "df4d00301812fd2d2eb9f063f7b2e3de1146007a",
          "url": "https://github.com/apytypes/apytypes/commit/6f1613718018f34beef7cc2801b3f740c395b1f8"
        },
        "date": 1710782954744,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4337120.250073418,
            "unit": "iter/sec",
            "range": "stddev: 1.750384145961214e-8",
            "extra": "mean: 230.56773673334575 nsec\nrounds: 193424"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1690621.7777281555,
            "unit": "iter/sec",
            "range": "stddev: 4.187315047420623e-8",
            "extra": "mean: 591.4983547317909 nsec\nrounds: 165810"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1990643.3888404479,
            "unit": "iter/sec",
            "range": "stddev: 3.583005539929016e-8",
            "extra": "mean: 502.35014749802093 nsec\nrounds: 197278"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1743619.9724733408,
            "unit": "iter/sec",
            "range": "stddev: 4.1766918642486646e-8",
            "extra": "mean: 573.5194685694541 nsec\nrounds: 171204"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1486591.3612211938,
            "unit": "iter/sec",
            "range": "stddev: 4.851087940227113e-8",
            "extra": "mean: 672.6798137576419 nsec\nrounds: 147016"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5304342.1088466,
            "unit": "iter/sec",
            "range": "stddev: 7.4275965794674755e-9",
            "extra": "mean: 188.52479336354244 nsec\nrounds: 53065"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5395962.297286445,
            "unit": "iter/sec",
            "range": "stddev: 6.79629172835725e-9",
            "extra": "mean: 185.32375596894929 nsec\nrounds: 52953"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9429412.43306334,
            "unit": "iter/sec",
            "range": "stddev: 5.098757811499085e-9",
            "extra": "mean: 106.05114656917333 nsec\nrounds: 95703"
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
          "id": "12af9d17ddfa5ee0d8bd3076a5cbdcd2a2b13d31",
          "message": "Add optmized two's overflow for single limb values",
          "timestamp": "2024-03-19T10:12:29+01:00",
          "tree_id": "5ecc3fa640bf11351eec27c66bf59994e2f9e629",
          "url": "https://github.com/apytypes/apytypes/commit/12af9d17ddfa5ee0d8bd3076a5cbdcd2a2b13d31"
        },
        "date": 1710839800263,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4483095.069135952,
            "unit": "iter/sec",
            "range": "stddev: 1.2151576022024371e-8",
            "extra": "mean: 223.0601815438733 nsec\nrounds: 44342"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1715183.7496044596,
            "unit": "iter/sec",
            "range": "stddev: 5.549420002156289e-8",
            "extra": "mean: 583.0279118669603 nsec\nrounds: 170329"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1862979.6598643456,
            "unit": "iter/sec",
            "range": "stddev: 5.525903045078779e-8",
            "extra": "mean: 536.7745131864206 nsec\nrounds: 185186"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1715681.6565816067,
            "unit": "iter/sec",
            "range": "stddev: 5.660981026838144e-8",
            "extra": "mean: 582.8587116752285 nsec\nrounds: 170329"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1638885.2981682902,
            "unit": "iter/sec",
            "range": "stddev: 5.5450544417511697e-8",
            "extra": "mean: 610.1708283780306 nsec\nrounds: 164420"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5735149.717410781,
            "unit": "iter/sec",
            "range": "stddev: 7.402873037729438e-9",
            "extra": "mean: 174.36336438859666 nsec\nrounds: 56456"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5666137.829726935,
            "unit": "iter/sec",
            "range": "stddev: 1.0287479493861976e-8",
            "extra": "mean: 176.48705874279676 nsec\nrounds: 57268"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10250332.807816233,
            "unit": "iter/sec",
            "range": "stddev: 6.471382128939163e-9",
            "extra": "mean: 97.55780799990168 nsec\nrounds: 102062"
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
          "id": "e0905d02079c6768e085064ac9a7b9292781ede5",
          "message": "Increase performance for fixed-point array negation",
          "timestamp": "2024-03-19T10:41:59+01:00",
          "tree_id": "537ea6009f257c3640ed92077a3a43073f449e4c",
          "url": "https://github.com/apytypes/apytypes/commit/e0905d02079c6768e085064ac9a7b9292781ede5"
        },
        "date": 1710841577734,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4387007.254869877,
            "unit": "iter/sec",
            "range": "stddev: 1.0241185641838855e-8",
            "extra": "mean: 227.94582773714228 nsec\nrounds: 44223"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1702687.1531797966,
            "unit": "iter/sec",
            "range": "stddev: 4.839568586287628e-8",
            "extra": "mean: 587.3069507410077 nsec\nrounds: 168011"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1809608.5428396557,
            "unit": "iter/sec",
            "range": "stddev: 4.434487742574965e-8",
            "extra": "mean: 552.6057024635699 nsec\nrounds: 178572"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1485240.8648183583,
            "unit": "iter/sec",
            "range": "stddev: 5.2771682046810855e-8",
            "extra": "mean: 673.2914665139573 nsec\nrounds: 146564"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1198721.13747153,
            "unit": "iter/sec",
            "range": "stddev: 5.562657519702102e-8",
            "extra": "mean: 834.2223797849323 nsec\nrounds: 119389"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5879938.671749223,
            "unit": "iter/sec",
            "range": "stddev: 8.81641080831945e-9",
            "extra": "mean: 170.0698010345713 nsec\nrounds: 57598"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5789010.4068722,
            "unit": "iter/sec",
            "range": "stddev: 1.2266007487198448e-8",
            "extra": "mean: 172.74109557876014 nsec\nrounds: 58371"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9917149.26139946,
            "unit": "iter/sec",
            "range": "stddev: 6.387913675209695e-9",
            "extra": "mean: 100.83542897681768 nsec\nrounds: 98630"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "committer": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "distinct": false,
          "id": "8fdcf13ac465a6ef94850f84f0c80dc9839539e0",
          "message": "Fix exception in multiplication",
          "timestamp": "2024-03-20T09:05:57+01:00",
          "tree_id": "dab4b4d2366191e3f34569bb6e583ce6d6aafc79",
          "url": "https://github.com/apytypes/apytypes/commit/8fdcf13ac465a6ef94850f84f0c80dc9839539e0"
        },
        "date": 1710922584998,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4364635.889900768,
            "unit": "iter/sec",
            "range": "stddev: 1.5658025748202435e-8",
            "extra": "mean: 229.11418620597914 nsec\nrounds: 43758"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1330993.5829950501,
            "unit": "iter/sec",
            "range": "stddev: 5.6387558079874916e-8",
            "extra": "mean: 751.3184231510278 nsec\nrounds: 132556"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4382090.834124454,
            "unit": "iter/sec",
            "range": "stddev: 1.0196991591012745e-8",
            "extra": "mean: 228.2015681219465 nsec\nrounds: 42637"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1258813.4995377252,
            "unit": "iter/sec",
            "range": "stddev: 5.4273186739805735e-8",
            "extra": "mean: 794.3988528620807 nsec\nrounds: 126663"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1256700.5983284556,
            "unit": "iter/sec",
            "range": "stddev: 5.408395060095342e-8",
            "extra": "mean: 795.7344822864918 nsec\nrounds: 125708"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1262082.125687385,
            "unit": "iter/sec",
            "range": "stddev: 6.905508563696259e-8",
            "extra": "mean: 792.3414646692053 nsec\nrounds: 123686"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1462713.160877578,
            "unit": "iter/sec",
            "range": "stddev: 5.116506836906265e-8",
            "extra": "mean: 683.6610394617542 nsec\nrounds: 145709"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1132193.1404568735,
            "unit": "iter/sec",
            "range": "stddev: 5.472213370825579e-8",
            "extra": "mean: 883.2415285579925 nsec\nrounds: 110902"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1241842.0780439766,
            "unit": "iter/sec",
            "range": "stddev: 5.277256486842787e-8",
            "extra": "mean: 805.2553683597481 nsec\nrounds: 119962"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5447481.779541467,
            "unit": "iter/sec",
            "range": "stddev: 7.87132512375759e-9",
            "extra": "mean: 183.5710591553673 nsec\nrounds: 55176"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5447101.691016605,
            "unit": "iter/sec",
            "range": "stddev: 7.499259809226442e-9",
            "extra": "mean: 183.58386839908067 nsec\nrounds: 54604"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9201461.837211376,
            "unit": "iter/sec",
            "range": "stddev: 7.290806815939623e-9",
            "extra": "mean: 108.67838368420719 nsec\nrounds: 91492"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "theodor.lindberg@liu.se",
            "name": "Theodor Lindberg",
            "username": "Theodor-Lindberg"
          },
          "committer": {
            "email": "40732757+Theodor-Lindberg@users.noreply.github.com",
            "name": "Theodor-Lindberg",
            "username": "Theodor-Lindberg"
          },
          "distinct": true,
          "id": "945083a637e09d72f4d413cef4e83360f300d646",
          "message": "Handle long formats in APyFloat power function",
          "timestamp": "2024-03-20T10:56:32+01:00",
          "tree_id": "d363a7dae42731cbd84aeadab196f860388baf8f",
          "url": "https://github.com/apytypes/apytypes/commit/945083a637e09d72f4d413cef4e83360f300d646"
        },
        "date": 1710928846861,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4153425.268009192,
            "unit": "iter/sec",
            "range": "stddev: 1.5557822599612604e-8",
            "extra": "mean: 240.76513611604128 nsec\nrounds: 43492"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1305373.4038151298,
            "unit": "iter/sec",
            "range": "stddev: 5.77214696730855e-8",
            "extra": "mean: 766.064328472911 nsec\nrounds: 128618"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4150463.9325063843,
            "unit": "iter/sec",
            "range": "stddev: 1.805878855245931e-8",
            "extra": "mean: 240.9369208508816 nsec\nrounds: 191976"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1250208.2201368795,
            "unit": "iter/sec",
            "range": "stddev: 6.17901656107394e-8",
            "extra": "mean: 799.8667613067661 nsec\nrounds: 122911"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1191109.895033505,
            "unit": "iter/sec",
            "range": "stddev: 6.606182051919957e-8",
            "extra": "mean: 839.5530959566787 nsec\nrounds: 117427"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1201661.047449599,
            "unit": "iter/sec",
            "range": "stddev: 5.51219781812564e-8",
            "extra": "mean: 832.1814226419483 nsec\nrounds: 120410"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1325575.749336202,
            "unit": "iter/sec",
            "range": "stddev: 7.338447236109659e-8",
            "extra": "mean: 754.3891780614929 nsec\nrounds: 130993"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1028179.9934142806,
            "unit": "iter/sec",
            "range": "stddev: 6.142258947318859e-8",
            "extra": "mean: 972.5923538731026 nsec\nrounds: 102797"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1017179.3480506705,
            "unit": "iter/sec",
            "range": "stddev: 7.310364667572988e-8",
            "extra": "mean: 983.1107974384607 nsec\nrounds: 100422"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5318573.305412874,
            "unit": "iter/sec",
            "range": "stddev: 9.58066994286443e-9",
            "extra": "mean: 188.02034729546108 nsec\nrounds: 52981"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5254540.910844998,
            "unit": "iter/sec",
            "range": "stddev: 1.0550101438780839e-8",
            "extra": "mean: 190.3115832509994 nsec\nrounds: 55146"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 8796314.616174763,
            "unit": "iter/sec",
            "range": "stddev: 7.0569364305842615e-9",
            "extra": "mean: 113.68397375880548 nsec\nrounds: 88410"
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
          "id": "c478d297ca4154ef10258fa8f0e5949310cc8476",
          "message": "Restructure code for readability",
          "timestamp": "2024-03-20T12:11:04+01:00",
          "tree_id": "ab5740e9d48836c176f83823695ab1ea4f7b0e73",
          "url": "https://github.com/apytypes/apytypes/commit/c478d297ca4154ef10258fa8f0e5949310cc8476"
        },
        "date": 1710933320134,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4034700.025422141,
            "unit": "iter/sec",
            "range": "stddev: 1.826356361268111e-8",
            "extra": "mean: 247.84990053757627 nsec\nrounds: 195695"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1310322.0136591229,
            "unit": "iter/sec",
            "range": "stddev: 5.227977561100469e-8",
            "extra": "mean: 763.1711820268581 nsec\nrounds: 128288"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4013942.067338651,
            "unit": "iter/sec",
            "range": "stddev: 2.6912626526948376e-8",
            "extra": "mean: 249.13164744881058 nsec\nrounds: 198847"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1261841.1368815745,
            "unit": "iter/sec",
            "range": "stddev: 9.747360527794118e-8",
            "extra": "mean: 792.4927875400766 nsec\nrounds: 122011"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1254590.952224743,
            "unit": "iter/sec",
            "range": "stddev: 5.934348354277719e-8",
            "extra": "mean: 797.0725424304276 nsec\nrounds: 120251"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1257279.3360268916,
            "unit": "iter/sec",
            "range": "stddev: 5.129412200279372e-8",
            "extra": "mean: 795.3681980967714 nsec\nrounds: 120106"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1113177.618720311,
            "unit": "iter/sec",
            "range": "stddev: 5.564137668182464e-8",
            "extra": "mean: 898.3292362179697 nsec\nrounds: 105619"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1224224.765093998,
            "unit": "iter/sec",
            "range": "stddev: 5.2940518651670776e-8",
            "extra": "mean: 816.8434657692994 nsec\nrounds: 120106"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1285647.0158878195,
            "unit": "iter/sec",
            "range": "stddev: 6.996998180857717e-8",
            "extra": "mean: 777.8184739995781 nsec\nrounds: 124456"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6165305.30068027,
            "unit": "iter/sec",
            "range": "stddev: 7.628081766837744e-9",
            "extra": "mean: 162.19796931870064 nsec\nrounds: 62186"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6171886.4022620665,
            "unit": "iter/sec",
            "range": "stddev: 8.324677632033213e-9",
            "extra": "mean: 162.02501712176044 nsec\nrounds: 59877"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 8693127.183911093,
            "unit": "iter/sec",
            "range": "stddev: 6.336451774344351e-9",
            "extra": "mean: 115.03340269198978 nsec\nrounds: 86267"
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
          "id": "68cddb2248c5f525387429bbf6d8ab139744fdfe",
          "message": "Add test for long integers and APyFloat",
          "timestamp": "2024-03-20T12:41:56+01:00",
          "tree_id": "67d63d3b788f6fbeb6a96785d86e2fc9a357b73a",
          "url": "https://github.com/apytypes/apytypes/commit/68cddb2248c5f525387429bbf6d8ab139744fdfe"
        },
        "date": 1710935199256,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4448032.003193752,
            "unit": "iter/sec",
            "range": "stddev: 9.476875302222344e-9",
            "extra": "mean: 224.81852632402402 nsec\nrounds: 43931"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1196592.2975767483,
            "unit": "iter/sec",
            "range": "stddev: 5.095158179910541e-8",
            "extra": "mean: 835.7065326470257 nsec\nrounds: 118681"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4436373.538464109,
            "unit": "iter/sec",
            "range": "stddev: 9.778843483942943e-9",
            "extra": "mean: 225.40933294501343 nsec\nrounds: 42875"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1216619.1557731554,
            "unit": "iter/sec",
            "range": "stddev: 1.246395951364503e-7",
            "extra": "mean: 821.9499054036245 nsec\nrounds: 123686"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1202848.0252481385,
            "unit": "iter/sec",
            "range": "stddev: 5.827764025421507e-8",
            "extra": "mean: 831.3602209170824 nsec\nrounds: 118414"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1204891.7903241704,
            "unit": "iter/sec",
            "range": "stddev: 5.7841757117563794e-8",
            "extra": "mean: 829.9500486520476 nsec\nrounds: 120251"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1387553.9694653063,
            "unit": "iter/sec",
            "range": "stddev: 6.095065340533086e-8",
            "extra": "mean: 720.6926880007057 nsec\nrounds: 136912"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1083164.8061236027,
            "unit": "iter/sec",
            "range": "stddev: 5.523726106797456e-8",
            "extra": "mean: 923.2205425680008 nsec\nrounds: 107562"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 983251.7316229012,
            "unit": "iter/sec",
            "range": "stddev: 6.753294972416416e-8",
            "extra": "mean: 1.0170335508582695 usec\nrounds: 97762"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5785772.891903905,
            "unit": "iter/sec",
            "range": "stddev: 8.322028468154465e-9",
            "extra": "mean: 172.83775541883662 nsec\nrounds: 56202"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5788352.236912136,
            "unit": "iter/sec",
            "range": "stddev: 9.809104977958283e-9",
            "extra": "mean: 172.7607372652349 nsec\nrounds: 58405"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 8800453.903482297,
            "unit": "iter/sec",
            "range": "stddev: 5.373266599304572e-9",
            "extra": "mean: 113.63050258168481 nsec\nrounds: 88881"
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
          "id": "0209a95a24ff85505f46fe4b2038518b930485d7",
          "message": "Refactor left-shift to speed up cast",
          "timestamp": "2024-03-20T19:59:06+01:00",
          "tree_id": "72307b75fd30328d7199e24cae5e9c1ee3752721",
          "url": "https://github.com/apytypes/apytypes/commit/0209a95a24ff85505f46fe4b2038518b930485d7"
        },
        "date": 1710961414556,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4419375.324311844,
            "unit": "iter/sec",
            "range": "stddev: 1.4570334497203471e-8",
            "extra": "mean: 226.27632337510914 nsec\nrounds: 46404"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1312946.156599,
            "unit": "iter/sec",
            "range": "stddev: 5.648789956447144e-8",
            "extra": "mean: 761.6458565142963 nsec\nrounds: 130651"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4447996.79595401,
            "unit": "iter/sec",
            "range": "stddev: 1.2374041881634874e-8",
            "extra": "mean: 224.82030583060367 nsec\nrounds: 45515"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1251179.191751202,
            "unit": "iter/sec",
            "range": "stddev: 5.659568469689995e-8",
            "extra": "mean: 799.2460285407767 nsec\nrounds: 126024"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1188827.232615407,
            "unit": "iter/sec",
            "range": "stddev: 5.887578099178404e-8",
            "extra": "mean: 841.1651185008833 nsec\nrounds: 120121"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1207741.377969786,
            "unit": "iter/sec",
            "range": "stddev: 7.007525527193058e-8",
            "extra": "mean: 827.9918352064849 nsec\nrounds: 121877"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1191979.171526086,
            "unit": "iter/sec",
            "range": "stddev: 5.681777333690906e-8",
            "extra": "mean: 838.9408337728838 nsec\nrounds: 120993"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1151829.7342940741,
            "unit": "iter/sec",
            "range": "stddev: 6.863158435232362e-8",
            "extra": "mean: 868.1838732118628 nsec\nrounds: 115795"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 977383.144928715,
            "unit": "iter/sec",
            "range": "stddev: 6.887204127282971e-8",
            "extra": "mean: 1.0231402139361996 usec\nrounds: 98339"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6071766.623411699,
            "unit": "iter/sec",
            "range": "stddev: 1.2584626396929952e-8",
            "extra": "mean: 164.6967121799879 nsec\nrounds: 60420"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6087410.635059789,
            "unit": "iter/sec",
            "range": "stddev: 8.290164148510806e-9",
            "extra": "mean: 164.27345877420223 nsec\nrounds: 60533"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9340804.29659049,
            "unit": "iter/sec",
            "range": "stddev: 8.139940127632663e-9",
            "extra": "mean: 107.05716212949697 nsec\nrounds: 94074"
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
          "id": "8ace95e9cfab1bebcb2e3ed7617d2f1c093d3ed2",
          "message": "Improve relative time plot",
          "timestamp": "2024-03-21T08:57:19+01:00",
          "tree_id": "57282aa4b07e4c18bfb69a65895ccda11a94ee88",
          "url": "https://github.com/apytypes/apytypes/commit/8ace95e9cfab1bebcb2e3ed7617d2f1c093d3ed2"
        },
        "date": 1711008190007,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4498237.5753482655,
            "unit": "iter/sec",
            "range": "stddev: 9.600076699525337e-9",
            "extra": "mean: 222.3092896382755 nsec\nrounds: 47173"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1222853.837428732,
            "unit": "iter/sec",
            "range": "stddev: 7.07202576845541e-8",
            "extra": "mean: 817.7592197793671 nsec\nrounds: 121581"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4402821.915852345,
            "unit": "iter/sec",
            "range": "stddev: 9.643338243847861e-9",
            "extra": "mean: 227.12706057891089 nsec\nrounds: 45577"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1340079.3144042986,
            "unit": "iter/sec",
            "range": "stddev: 4.9720757179922236e-8",
            "extra": "mean: 746.2244877979218 nsec\nrounds: 121877"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1237737.8314935963,
            "unit": "iter/sec",
            "range": "stddev: 5.3349930977633704e-8",
            "extra": "mean: 807.9255352429251 nsec\nrounds: 121581"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1251680.5231056975,
            "unit": "iter/sec",
            "range": "stddev: 5.3570167897787426e-8",
            "extra": "mean: 798.9259092397873 nsec\nrounds: 123840"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1472331.6690893262,
            "unit": "iter/sec",
            "range": "stddev: 6.222109317999359e-8",
            "extra": "mean: 679.1947908167606 nsec\nrounds: 143823"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1372363.7909942898,
            "unit": "iter/sec",
            "range": "stddev: 5.2114399237007056e-8",
            "extra": "mean: 728.6697642142663 nsec\nrounds: 131857"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 947432.1054784183,
            "unit": "iter/sec",
            "range": "stddev: 6.56293320680493e-8",
            "extra": "mean: 1.0554846032951717 usec\nrounds: 93546"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5984314.5399510795,
            "unit": "iter/sec",
            "range": "stddev: 7.715580841492116e-9",
            "extra": "mean: 167.1035159205704 nsec\nrounds: 62035"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5940110.098171171,
            "unit": "iter/sec",
            "range": "stddev: 7.400907413178803e-9",
            "extra": "mean: 168.34704802998027 nsec\nrounds: 61691"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9255358.400533836,
            "unit": "iter/sec",
            "range": "stddev: 5.866072811378935e-9",
            "extra": "mean: 108.04551879291934 nsec\nrounds: 90335"
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
          "id": "081a78a10f101f7f997541da8235bf4ddd6aac6b",
          "message": "Improve relative time plot",
          "timestamp": "2024-03-21T10:00:07+01:00",
          "tree_id": "6656184cadd3dc10214fa488fce7605587fbd584",
          "url": "https://github.com/apytypes/apytypes/commit/081a78a10f101f7f997541da8235bf4ddd6aac6b"
        },
        "date": 1711011951594,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4508131.195871583,
            "unit": "iter/sec",
            "range": "stddev: 1.1858703155177334e-8",
            "extra": "mean: 221.821405933256 nsec\nrounds: 47015"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1317237.787484497,
            "unit": "iter/sec",
            "range": "stddev: 6.16125254806134e-8",
            "extra": "mean: 759.1643737381797 nsec\nrounds: 130822"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4558534.290106928,
            "unit": "iter/sec",
            "range": "stddev: 9.538826186447492e-9",
            "extra": "mean: 219.36875678877658 nsec\nrounds: 46211"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1283807.2742477343,
            "unit": "iter/sec",
            "range": "stddev: 6.143216424906214e-8",
            "extra": "mean: 778.933115631375 nsec\nrounds: 125708"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1241687.4774241757,
            "unit": "iter/sec",
            "range": "stddev: 5.7983893351694956e-8",
            "extra": "mean: 805.3556294811914 nsec\nrounds: 124301"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1220591.7928872667,
            "unit": "iter/sec",
            "range": "stddev: 6.580687654369401e-8",
            "extra": "mean: 819.27472053088 nsec\nrounds: 124611"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1393063.163662023,
            "unit": "iter/sec",
            "range": "stddev: 6.80449500877556e-8",
            "extra": "mean: 717.8425401553382 nsec\nrounds: 136370"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1255699.671810783,
            "unit": "iter/sec",
            "range": "stddev: 5.915574437534783e-8",
            "extra": "mean: 796.3687675078894 nsec\nrounds: 122760"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1112458.1489036141,
            "unit": "iter/sec",
            "range": "stddev: 6.065066227902829e-8",
            "extra": "mean: 898.910220564718 nsec\nrounds: 107910"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5859932.35666037,
            "unit": "iter/sec",
            "range": "stddev: 1.0824894959945306e-8",
            "extra": "mean: 170.6504340213945 nsec\nrounds: 57831"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5796421.421803972,
            "unit": "iter/sec",
            "range": "stddev: 1.0343720076837212e-8",
            "extra": "mean: 172.52023744138614 nsec\nrounds: 61573"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9414936.988673251,
            "unit": "iter/sec",
            "range": "stddev: 6.167563247734689e-9",
            "extra": "mean: 106.2141999678456 nsec\nrounds: 92765"
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
          "distinct": true,
          "id": "5fb5aee480e9440149e548ca399b3aa061218a04",
          "message": "src: fix APyFixed.__div__, closes #198",
          "timestamp": "2024-03-21T11:06:59+01:00",
          "tree_id": "ad81da3d6efed12f76dd5de6fed4d737efe06050",
          "url": "https://github.com/apytypes/apytypes/commit/5fb5aee480e9440149e548ca399b3aa061218a04"
        },
        "date": 1711016003726,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4302381.266110642,
            "unit": "iter/sec",
            "range": "stddev: 1.1767241250516547e-8",
            "extra": "mean: 232.4294241137586 nsec\nrounds: 44679"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1602290.9413445394,
            "unit": "iter/sec",
            "range": "stddev: 4.7420659634490965e-8",
            "extra": "mean: 624.1063805558733 nsec\nrounds: 161005"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4265297.880042298,
            "unit": "iter/sec",
            "range": "stddev: 1.1994188711868525e-8",
            "extra": "mean: 234.45021382425844 nsec\nrounds: 43587"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1386726.9225846082,
            "unit": "iter/sec",
            "range": "stddev: 6.446166982027131e-8",
            "extra": "mean: 721.1225106498935 nsec\nrounds: 139978"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1332088.0334695692,
            "unit": "iter/sec",
            "range": "stddev: 5.7931255799839835e-8",
            "extra": "mean: 750.7011360168058 nsec\nrounds: 131510"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1342752.0728670452,
            "unit": "iter/sec",
            "range": "stddev: 5.432265348575569e-8",
            "extra": "mean: 744.7391221409736 nsec\nrounds: 133441"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1793083.4120998185,
            "unit": "iter/sec",
            "range": "stddev: 4.6359183340251613e-8",
            "extra": "mean: 557.6985394276323 nsec\nrounds: 174521"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1655861.350234071,
            "unit": "iter/sec",
            "range": "stddev: 4.842296350410614e-8",
            "extra": "mean: 603.9152975330014 nsec\nrounds: 165262"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1647069.32137612,
            "unit": "iter/sec",
            "range": "stddev: 4.7815068350352756e-8",
            "extra": "mean: 607.1389874255548 nsec\nrounds: 162814"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5740576.329684242,
            "unit": "iter/sec",
            "range": "stddev: 1.2415211395576012e-8",
            "extra": "mean: 174.19853731916868 nsec\nrounds: 57627"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5686725.860352682,
            "unit": "iter/sec",
            "range": "stddev: 1.0868452029022896e-8",
            "extra": "mean: 175.84811094409466 nsec\nrounds: 57166"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10426595.446916034,
            "unit": "iter/sec",
            "range": "stddev: 6.787753417681108e-9",
            "extra": "mean: 95.90858349605168 nsec\nrounds: 104080"
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
          "id": "6550e822faa3b418af34707dae8dae7edaab1107",
          "message": "Specialize conversion of numpy arrays to APyFloatArray",
          "timestamp": "2024-03-21T12:11:40+01:00",
          "tree_id": "af97207994a9f565b7b75c15f425f71655c29f1b",
          "url": "https://github.com/apytypes/apytypes/commit/6550e822faa3b418af34707dae8dae7edaab1107"
        },
        "date": 1711019763922,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4671810.4677971,
            "unit": "iter/sec",
            "range": "stddev: 9.09322326449595e-9",
            "extra": "mean: 214.04977939341677 nsec\nrounds: 46599"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1618244.4838178186,
            "unit": "iter/sec",
            "range": "stddev: 4.501148196086227e-8",
            "extra": "mean: 617.9535972467982 nsec\nrounds: 160206"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4254836.526857817,
            "unit": "iter/sec",
            "range": "stddev: 1.0415381053838446e-8",
            "extra": "mean: 235.0266558274354 nsec\nrounds: 45515"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1365366.1702089645,
            "unit": "iter/sec",
            "range": "stddev: 5.8401899924035693e-8",
            "extra": "mean: 732.4042603508706 nsec\nrounds: 136538"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1311081.1172482993,
            "unit": "iter/sec",
            "range": "stddev: 1.257021596769597e-7",
            "extra": "mean: 762.7293131173798 nsec\nrounds: 130822"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1346407.2670567962,
            "unit": "iter/sec",
            "range": "stddev: 7.841587630592937e-8",
            "extra": "mean: 742.7173222155424 nsec\nrounds: 132206"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1670295.4976514531,
            "unit": "iter/sec",
            "range": "stddev: 4.7473827725770076e-8",
            "extra": "mean: 598.6964590433354 nsec\nrounds: 164447"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1732308.668388275,
            "unit": "iter/sec",
            "range": "stddev: 4.459980266037964e-8",
            "extra": "mean: 577.2643283776861 nsec\nrounds: 171792"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1577584.6439545648,
            "unit": "iter/sec",
            "range": "stddev: 8.968590798244447e-8",
            "extra": "mean: 633.8804094170699 nsec\nrounds: 158178"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5896712.879343626,
            "unit": "iter/sec",
            "range": "stddev: 8.241992198374967e-9",
            "extra": "mean: 169.58600841885593 nsec\nrounds: 56777"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5935598.622997924,
            "unit": "iter/sec",
            "range": "stddev: 1.1244435725419084e-8",
            "extra": "mean: 168.47500370484818 nsec\nrounds: 59376"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10348178.31654699,
            "unit": "iter/sec",
            "range": "stddev: 5.631671547335989e-9",
            "extra": "mean: 96.63536609153428 nsec\nrounds: 103649"
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
          "id": "f02a6dea59557a08d0a5fec7b472e2941500e18b",
          "message": "Speed-up array conversion for APyFloat",
          "timestamp": "2024-03-21T14:16:09+01:00",
          "tree_id": "ac6b39dffe2c2b3c5beadd22b11b7c1fc96fdde7",
          "url": "https://github.com/apytypes/apytypes/commit/f02a6dea59557a08d0a5fec7b472e2941500e18b"
        },
        "date": 1711027270461,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4627827.234635088,
            "unit": "iter/sec",
            "range": "stddev: 1.2841114641328889e-8",
            "extra": "mean: 216.08412529229204 nsec\nrounds: 47170"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1590826.966237895,
            "unit": "iter/sec",
            "range": "stddev: 6.427688538091107e-8",
            "extra": "mean: 628.6038778717053 nsec\nrounds: 159211"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4499228.262292896,
            "unit": "iter/sec",
            "range": "stddev: 9.580712830860381e-9",
            "extra": "mean: 222.26033926322242 nsec\nrounds: 46041"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1351107.5339743579,
            "unit": "iter/sec",
            "range": "stddev: 5.5733874653833296e-8",
            "extra": "mean: 740.1335384892716 nsec\nrounds: 134699"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1312173.0273016477,
            "unit": "iter/sec",
            "range": "stddev: 5.049187577922609e-8",
            "extra": "mean: 762.0946164824229 nsec\nrounds: 129618"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1310155.7722604296,
            "unit": "iter/sec",
            "range": "stddev: 5.3158945462155045e-8",
            "extra": "mean: 763.2680183324608 nsec\nrounds: 129618"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1759003.3986780073,
            "unit": "iter/sec",
            "range": "stddev: 6.05887046548562e-8",
            "extra": "mean: 568.5037338481254 nsec\nrounds: 175717"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1681758.494362813,
            "unit": "iter/sec",
            "range": "stddev: 5.286140214999569e-8",
            "extra": "mean: 594.6156974095086 nsec\nrounds: 166639"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1492092.7409833733,
            "unit": "iter/sec",
            "range": "stddev: 5.03308785042105e-8",
            "extra": "mean: 670.1996280344808 nsec\nrounds: 147864"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 5639070.05516311,
            "unit": "iter/sec",
            "range": "stddev: 7.460763308241583e-9",
            "extra": "mean: 177.3342040828434 nsec\nrounds: 55948"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 5463284.8269758485,
            "unit": "iter/sec",
            "range": "stddev: 1.1045530068546332e-8",
            "extra": "mean: 183.04006319831615 nsec\nrounds: 55698"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9925890.76430066,
            "unit": "iter/sec",
            "range": "stddev: 5.5438509090722964e-9",
            "extra": "mean: 100.74662554186875 nsec\nrounds: 98727"
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
          "id": "bc76fdc0180387ded747b632f4283b7a7bf1e03d",
          "message": "Remove dead code",
          "timestamp": "2024-03-21T14:26:40+01:00",
          "tree_id": "49d0b8724c1964f2c690cc41a2a0cb7d29ca508b",
          "url": "https://github.com/apytypes/apytypes/commit/bc76fdc0180387ded747b632f4283b7a7bf1e03d"
        },
        "date": 1711027885039,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4472873.262164434,
            "unit": "iter/sec",
            "range": "stddev: 2.7267307956561236e-8",
            "extra": "mean: 223.5699384685666 nsec\nrounds: 44205"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1493799.8624409526,
            "unit": "iter/sec",
            "range": "stddev: 4.883956980466124e-8",
            "extra": "mean: 669.433720770294 nsec\nrounds: 142390"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4453694.686094922,
            "unit": "iter/sec",
            "range": "stddev: 1.0103667697834917e-8",
            "extra": "mean: 224.53267915336437 nsec\nrounds: 44264"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1340259.01770024,
            "unit": "iter/sec",
            "range": "stddev: 1.3372713143696888e-7",
            "extra": "mean: 746.124433257592 nsec\nrounds: 133085"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1216587.0230822517,
            "unit": "iter/sec",
            "range": "stddev: 9.73039707428204e-8",
            "extra": "mean: 821.9716148759094 nsec\nrounds: 124611"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1272113.4433500636,
            "unit": "iter/sec",
            "range": "stddev: 6.46160974406051e-8",
            "extra": "mean: 786.0934142528567 nsec\nrounds: 125392"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1586209.6020402573,
            "unit": "iter/sec",
            "range": "stddev: 7.740261708465337e-8",
            "extra": "mean: 630.4337073195206 nsec\nrounds: 162840"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1544032.931396102,
            "unit": "iter/sec",
            "range": "stddev: 5.990365024243894e-8",
            "extra": "mean: 647.6545802011211 nsec\nrounds: 144655"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1642989.8400118423,
            "unit": "iter/sec",
            "range": "stddev: 4.9926256577570533e-8",
            "extra": "mean: 608.6464904693488 nsec\nrounds: 158454"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6784261.47000203,
            "unit": "iter/sec",
            "range": "stddev: 8.805558507842017e-9",
            "extra": "mean: 147.39997926402913 nsec\nrounds: 67034"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6549435.365004902,
            "unit": "iter/sec",
            "range": "stddev: 9.807899140147676e-9",
            "extra": "mean: 152.6849177477798 nsec\nrounds: 67354"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 9932542.976164721,
            "unit": "iter/sec",
            "range": "stddev: 6.050929127526144e-9",
            "extra": "mean: 100.67915159290136 nsec\nrounds: 100011"
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
          "id": "1879554678b31d0089d3f976add183a1b029fa30",
          "message": "src: APyFixed::operator/() specialize for single limb",
          "timestamp": "2024-03-21T14:34:43+01:00",
          "tree_id": "51245ab8c4db8f2314cda6d31af54d00c25044e6",
          "url": "https://github.com/apytypes/apytypes/commit/1879554678b31d0089d3f976add183a1b029fa30"
        },
        "date": 1711028702631,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4540201.986161206,
            "unit": "iter/sec",
            "range": "stddev: 1.563464409919035e-8",
            "extra": "mean: 220.25451798130553 nsec\nrounds: 45268"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1628911.4503115423,
            "unit": "iter/sec",
            "range": "stddev: 5.143029951003648e-8",
            "extra": "mean: 613.9069129932053 nsec\nrounds: 159439"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4500262.82540345,
            "unit": "iter/sec",
            "range": "stddev: 1.0093679287845969e-8",
            "extra": "mean: 222.20924394795512 nsec\nrounds: 44362"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1344528.6322554569,
            "unit": "iter/sec",
            "range": "stddev: 5.965557693367564e-8",
            "extra": "mean: 743.755079668662 nsec\nrounds: 135796"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1307901.6179480297,
            "unit": "iter/sec",
            "range": "stddev: 8.812499732699572e-8",
            "extra": "mean: 764.5835025182902 nsec\nrounds: 130141"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1261634.9016240742,
            "unit": "iter/sec",
            "range": "stddev: 1.4103211571352733e-7",
            "extra": "mean: 792.6223336979077 nsec\nrounds: 131165"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1664747.1751344332,
            "unit": "iter/sec",
            "range": "stddev: 5.206046880597345e-8",
            "extra": "mean: 600.6918137098809 nsec\nrounds: 166058"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1682303.3994032524,
            "unit": "iter/sec",
            "range": "stddev: 1.6329245393519879e-7",
            "extra": "mean: 594.4230989217897 nsec\nrounds: 168891"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1298965.857794149,
            "unit": "iter/sec",
            "range": "stddev: 6.890614586329989e-8",
            "extra": "mean: 769.8431748607971 nsec\nrounds: 127470"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6228934.817792575,
            "unit": "iter/sec",
            "range": "stddev: 7.810794944863438e-9",
            "extra": "mean: 160.5410923780381 nsec\nrounds: 62579"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6218286.802992788,
            "unit": "iter/sec",
            "range": "stddev: 1.066926253064514e-8",
            "extra": "mean: 160.81599830979874 nsec\nrounds: 62661"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10188177.787988015,
            "unit": "iter/sec",
            "range": "stddev: 8.481252006909366e-9",
            "extra": "mean: 98.15297895365163 nsec\nrounds: 99921"
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
          "distinct": true,
          "id": "16fa3b966d54c3882e6d9c770fbe9dad72203278",
          "message": "src: APyFixed.cc, explicit template instantiation",
          "timestamp": "2024-03-21T14:42:13+01:00",
          "tree_id": "7dbac5963ee33f6aa5c65d66c0cb57c87508bfe3",
          "url": "https://github.com/apytypes/apytypes/commit/16fa3b966d54c3882e6d9c770fbe9dad72203278"
        },
        "date": 1711028957066,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_short",
            "value": 4559326.487971798,
            "unit": "iter/sec",
            "range": "stddev: 9.32538981714676e-9",
            "extra": "mean: 219.3306407510178 nsec\nrounds: 45041"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_long",
            "value": 1600930.7083363542,
            "unit": "iter/sec",
            "range": "stddev: 6.427394480598582e-8",
            "extra": "mean: 624.6366534121761 nsec\nrounds: 158932"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_multiplication_mixed",
            "value": 4541852.153725386,
            "unit": "iter/sec",
            "range": "stddev: 9.519320991413603e-9",
            "extra": "mean: 220.1744940507773 nsec\nrounds: 44461"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_short",
            "value": 1367165.0316070856,
            "unit": "iter/sec",
            "range": "stddev: 5.8356040188386026e-8",
            "extra": "mean: 731.440591941192 nsec\nrounds: 134337"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_long",
            "value": 1338074.995470123,
            "unit": "iter/sec",
            "range": "stddev: 5.6845066553894615e-8",
            "extra": "mean: 747.342266603375 nsec\nrounds: 130124"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_division_mixed",
            "value": 1341193.5894067427,
            "unit": "iter/sec",
            "range": "stddev: 5.488808077611124e-8",
            "extra": "mean: 745.6045181682392 nsec\nrounds: 130805"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_short",
            "value": 1688294.1103883504,
            "unit": "iter/sec",
            "range": "stddev: 4.9965583467511724e-8",
            "extra": "mean: 592.3138592065781 nsec\nrounds: 165262"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_long",
            "value": 1521136.4239956639,
            "unit": "iter/sec",
            "range": "stddev: 5.390130208433536e-8",
            "extra": "mean: 657.4032310482478 nsec\nrounds: 149858"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_addition_mixed",
            "value": 1592982.1938355262,
            "unit": "iter/sec",
            "range": "stddev: 5.338028364768919e-8",
            "extra": "mean: 627.753407332341 nsec\nrounds: 158203"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_short",
            "value": 6217197.8222482875,
            "unit": "iter/sec",
            "range": "stddev: 2.3152018057950348e-8",
            "extra": "mean: 160.8441662289188 nsec\nrounds: 192679"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_creation_long",
            "value": 6585000.287757374,
            "unit": "iter/sec",
            "range": "stddev: 9.66609889303129e-9",
            "extra": "mean: 151.8602818984412 nsec\nrounds: 67400"
          },
          {
            "name": "benchmark/ci_float_scalar_benchmarks.py::test_float_scalar_to_float_short",
            "value": 10203157.875062956,
            "unit": "iter/sec",
            "range": "stddev: 6.502710738846421e-9",
            "extra": "mean: 98.00887257110801 nsec\nrounds: 102481"
          }
        ]
      }
    ]
  }
}