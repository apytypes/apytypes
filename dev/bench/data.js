window.BENCHMARK_DATA = {
  "lastUpdate": 1710457445712,
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
          "id": "3645c23d83f768684f1de972d635f7bb935cf0b2",
          "message": "Add floating-point array benchmarks",
          "timestamp": "2024-03-14T23:32:28+01:00",
          "tree_id": "f945074951f1f6976717904c6ba06f1f80377632",
          "url": "https://github.com/apytypes/apytypes/commit/3645c23d83f768684f1de972d635f7bb935cf0b2"
        },
        "date": 1710455747693,
        "tool": "pytest",
        "benches": [
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_fixed_matrix_multiplication_20",
            "value": 173.7251603684282,
            "unit": "iter/sec",
            "range": "stddev: 0.00005068398520819182",
            "extra": "mean: 5.7562186034495335 msec\nrounds: 174"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_fixed_matrix_addition_20",
            "value": 5526.385675630962,
            "unit": "iter/sec",
            "range": "stddev: 0.000005393899855892448",
            "extra": "mean: 180.9500926454662 usec\nrounds: 5494"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_fixed_matrix_addition_200",
            "value": 53.820041335749174,
            "unit": "iter/sec",
            "range": "stddev: 0.0006154553987502923",
            "extra": "mean: 18.580439092598105 msec\nrounds: 54"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_20",
            "value": 8639.6800475065,
            "unit": "iter/sec",
            "range": "stddev: 0.000010656529852402897",
            "extra": "mean: 115.74502695717423 usec\nrounds: 8532"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_fixed_matrix_elementwise_multiplication_200",
            "value": 87.54776545152939,
            "unit": "iter/sec",
            "range": "stddev: 0.00014454335046830505",
            "extra": "mean: 11.422336079538747 msec\nrounds: 88"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_fixed_scalar_matrix_addition_20",
            "value": 6531.286384959214,
            "unit": "iter/sec",
            "range": "stddev: 0.0000030519119562487195",
            "extra": "mean: 153.10919489044036 usec\nrounds: 6537"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_fixed_scalar_matrix_addition_200",
            "value": 64.92245398470664,
            "unit": "iter/sec",
            "range": "stddev: 0.00016256688655726453",
            "extra": "mean: 15.402991393941509 msec\nrounds: 66"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_20",
            "value": 9388.941351100913,
            "unit": "iter/sec",
            "range": "stddev: 0.000003425312596604586",
            "extra": "mean: 106.5082806042604 usec\nrounds: 9198"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_fixed_scalar_matrix_multiplication_200",
            "value": 94.18352359903145,
            "unit": "iter/sec",
            "range": "stddev: 0.00010531456387018373",
            "extra": "mean: 10.617568357893584 msec\nrounds: 95"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_fixed_matrix_creation_20",
            "value": 9544.439772643098,
            "unit": "iter/sec",
            "range": "stddev: 0.0000064732273309806845",
            "extra": "mean: 104.77304313515245 usec\nrounds: 9505"
          },
          {
            "name": "benchmark/ci_float_array_benchmarks.py::test_fixed_matrix_creation_200",
            "value": 141.08693956699014,
            "unit": "iter/sec",
            "range": "stddev: 0.00012728342657758",
            "extra": "mean: 7.087828278571351 msec\nrounds: 140"
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
      }
    ]
  }
}