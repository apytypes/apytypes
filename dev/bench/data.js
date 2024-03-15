window.BENCHMARK_DATA = {
  "lastUpdate": 1710496010030,
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
      }
    ]
  }
}