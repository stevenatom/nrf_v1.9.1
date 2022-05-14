/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <sys/util.h>

#include "mcc_location_table.h"

/* Float latitude to integer conversion factor (2^23/90) */
#define LAT_CONV (8388608.0f / 90.0f)
/* Float longitude to integer conversion factor (2^24/360) */
#define LON_CONV (16777216.0f / 360.0f)

static const struct mcc_table mcc_table[] = {
	{ 100, 104, 104,   0,   43.00f,   41.01f, 289 }, /* Abkhazia */
	{ 100, 119, 119,   0,   33.84f,   66.00f, 412 }, /* Afghanistan */
	{ 100, 104, 104,   0,   41.14f,   20.05f, 276 }, /* Albania */
	{ 100, 125, 125,   0,   28.16f,    2.62f, 603 }, /* Algeria */
	{ 100, 107, 107,   0,  -14.31f, -170.70f, 544 }, /* American Samoa */
	{ 100,  80,  80,   0,   42.54f,    1.56f, 213 }, /* Andorra */
	{ 100, 121, 121,   0,  -11.21f,   17.88f, 631 }, /* Angola */
	{ 100,  86,  86,   0,   18.22f,  -63.06f, 365 }, /* Anguilla */
	{ 100,  88,  88,   0,   17.28f,  -61.79f, 344 }, /* Antigua and Barbuda */
	{  80, 118, 127,   0,  -35.38f,  -65.18f, 722 }, /* Argentina */
	{ 100, 104, 104,   0,   40.29f,   44.93f, 283 }, /* Armenia */
	{ 100,  78,  78,   0,   12.52f,  -69.96f, 363 }, /* Aruba */
	{  80, 127, 127,   0,  -25.73f,  134.49f, 505 }, /* Australia */
	{ 100, 109, 109,   0,   47.59f,   14.13f, 232 }, /* Austria */
	{ 100, 109, 109,   0,   40.29f,   47.55f, 400 }, /* Azerbaijan */
	{ 100, 110, 113,   0,   24.29f,  -76.63f, 364 }, /* Bahamas */
	{ 100,  84,  84,   0,   26.04f,   50.54f, 426 }, /* Bahrain */
	{ 100, 112, 112,   0,   23.87f,   90.24f, 470 }, /* Bangladesh */
	{ 100,  81,  81,   0,   13.18f,  -59.56f, 342 }, /* Barbados */
	{ 100, 112, 112,   0,   53.53f,   28.03f, 257 }, /* Belarus */
	{ 100, 103, 103,   0,   50.64f,    4.64f, 206 }, /* Belgium */
	{ 100, 102, 102,   0,   17.20f,  -88.71f, 702 }, /* Belize */
	{ 100, 111, 111,   0,    9.64f,    2.33f, 616 }, /* Benin */
	{ 100,  77,  77,   0,   32.31f,  -64.75f, 350 }, /* Bermuda */
	{ 100, 104, 104,   0,   27.41f,   90.40f, 402 }, /* Bhutan */
	{ 100, 121, 121,   0,  -16.71f,  -64.69f, 736 }, /* Bolivia */
	{ 100, 105, 105,   0,   44.17f,   17.77f, 218 }, /* Bosnia and Herzegovina */
	{ 100, 118, 118,   0,  -22.18f,   23.80f, 652 }, /* Botswana */
	{  66, 127, 127,   0,  -10.79f,  -53.10f, 724 }, /* Brazil */
	{ 100, 101, 101,   0,   -7.33f,   72.42f, 995 }, /* British Indian Ocean Territory */
	{ 100,  86,  86,   0,   18.42f,  -64.59f, 348 }, /* British Virgin Islands */
	{ 100,  97,  97,   0,    4.52f,  114.72f, 528 }, /* Brunei */
	{ 100, 109, 109,   0,   42.77f,   25.22f, 284 }, /* Bulgaria */
	{ 100, 115, 115,   0,   12.27f,   -1.75f, 613 }, /* Burkina Faso */
	{ 100, 102, 102,   0,   -3.36f,   29.88f, 642 }, /* Burundi */
	{ 100, 111, 111,   0,   12.72f,  104.91f, 456 }, /* Cambodia */
	{ 100, 119, 119,   0,    5.69f,   12.74f, 624 }, /* Cameroon */
	{  33, 127, 127,   0,   61.36f,  -98.31f, 302 }, /* Canada */
	{ 100, 102, 102,   0,   15.96f,  -23.96f, 625 }, /* Cape Verde */
	{ 100,  96,  96,   0,   19.43f,  -80.91f, 346 }, /* Cayman Islands */
	{ 100, 120, 120,   0,    6.57f,   20.47f, 623 }, /* Central African Republic */
	{ 100, 122, 122,   0,   15.33f,   18.64f, 622 }, /* Chad */
	{  75, 127, 127,   0,  -37.73f,  -71.38f, 730 }, /* Chile */
	{  33, 127, 127,   0,   36.56f,  103.82f, 460 }, /* China */
	{  33, 127, 127,   0,   36.56f,  103.82f, 461 }, /* China */
	{ 100, 124, 124,   0,    3.91f,  -73.08f, 732 }, /* Colombia */
	{ 100,  96,  96,   0,  -11.88f,   43.68f, 654 }, /* Comoros */
	{ 100, 117, 117,   0,   -0.84f,   15.22f, 629 }, /* Congo */
	{ 100, 120, 120,   0,  -21.22f, -159.79f, 548 }, /* Cook Islands */
	{ 100, 112, 112,   0,    9.98f,  -84.19f, 712 }, /* Costa Rica */
	{ 100, 110, 110,   0,   45.08f,   16.40f, 219 }, /* Croatia */
	{ 100, 116, 116,   0,   21.62f,  -79.02f, 368 }, /* Cuba */
	{ 100,  87,  87,   0,   12.20f,  -68.97f, 362 }, /* Curacao */
	{ 100,  96,  96,   0,   34.92f,   33.01f, 280 }, /* Cyprus */
	{ 100, 108, 108,   0,   49.73f,   15.31f, 230 }, /* Czech Republic */
	{ 100, 125, 125,   0,   -2.88f,   23.64f, 630 }, /* Democratic Republic of the Congo */
	{ 100, 108, 108,   0,   55.98f,   10.03f, 238 }, /* Denmark */
	{ 100, 100, 100,   0,   11.75f,   42.56f, 638 }, /* Djibouti */
	{ 100,  84,  84,   0,   15.44f,  -61.36f, 366 }, /* Dominica */
	{ 100, 106, 106,   0,   18.89f,  -70.51f, 370 }, /* Dominican Republic */
	{ 100, 104, 104,   0,   -8.79f,  126.14f, 514 }, /* East Timor */
	{ 100, 121, 121,   0,   -1.42f,  -78.75f, 740 }, /* Ecuador */
	{ 100, 119, 119,   0,   26.50f,   29.86f, 602 }, /* Egypt */
	{ 100, 101, 101,   0,   13.74f,  -88.87f, 706 }, /* El Salvador */
	{ 100, 112, 112,   0,    1.62f,   10.32f, 627 }, /* Equatorial Guinea */
	{ 100, 114, 114,   0,   15.36f,   38.85f, 657 }, /* Eritrea */
	{ 100, 105, 105,   0,   58.67f,   25.54f, 248 }, /* Estonia */
	{ 100, 122, 122,   0,    8.62f,   39.60f, 636 }, /* Ethiopia */
	{ 100, 101, 101,   0,  -51.74f,  -59.35f, 750 }, /* Falkland Islands */
	{ 100,  93,  93,   0,   62.05f,   -6.88f, 288 }, /* Faroe Islands */
	{ 100, 117, 117,   0,  -17.43f,  165.45f, 542 }, /* Fiji */
	{ 100, 116, 116,   0,   64.50f,   26.27f, 244 }, /* Finland */
	{  90, 117, 117,   0,   42.17f,   -2.76f, 208 }, /* France */
	{ 100, 104, 104,   0,    3.93f,  -53.09f, 742 }, /* French Guiana */
	{  50, 127, 127,   0,  -21.13f,   55.53f, 647 }, /* French Indian Ocean Territories */
	{ 100, 125, 125,   0,  -17.69f, -149.37f, 547 }, /* French Polynesia */
	{ 100, 113, 113,   0,   -0.59f,   11.79f, 628 }, /* Gabon */
	{ 100, 103, 103,   0,   13.45f,  -15.40f, 607 }, /* Gambia */
	{ 100, 109, 109,   0,   42.17f,   43.51f, 282 }, /* Georgia */
	{ 100, 115, 115,   0,   51.11f,   10.39f, 262 }, /* Germany */
	{ 100, 113, 113,   0,    7.95f,   -1.22f, 620 }, /* Ghana */
	{ 100,  59,  59,   0,   36.14f,   -5.35f, 266 }, /* Gibraltar */
	{ 100, 115, 115,   0,   39.07f,   22.96f, 202 }, /* Greece */
	{ 100, 126, 126,   0,   74.71f,  -41.34f, 290 }, /* Greenland */
	{ 100,  86,  86,   0,   12.12f,  -61.68f, 352 }, /* Grenada */
	{ 100,  90,  90,   0,   16.17f,  -61.41f, 340 }, /* Guadeloupe */
	{ 100, 109, 109,   0,   15.69f,  -90.36f, 704 }, /* Guatemala */
	{ 100, 114, 114,   0,   10.44f,  -10.94f, 611 }, /* Guinea */
	{ 100, 104, 104,   0,   12.05f,  -14.95f, 632 }, /* Guinea-Bissau */
	{ 100, 114, 114,   0,    4.79f,  -58.98f, 738 }, /* Guyana */
	{ 100, 104, 104,   0,   18.94f,  -72.69f, 372 }, /* Haiti */
	{ 100, 112, 112,   0,   14.83f,  -86.62f, 708 }, /* Honduras */
	{ 100,  87,  87,   0,   22.40f,  114.11f, 454 }, /* Hong Kong */
	{ 100, 109, 109,   0,   47.16f,   19.40f, 216 }, /* Hungary */
	{ 100, 109, 109,   0,   65.00f,  -18.57f, 274 }, /* Iceland */
	{ 100, 125, 125,   0,   22.89f,   79.61f, 404 }, /* India */
	{ 100, 125, 125,   0,   22.89f,   79.61f, 405 }, /* India */
	{ 100, 125, 125,   0,   22.89f,   79.61f, 406 }, /* India */
	{  66, 121, 127,   0,   -2.22f,  117.24f, 510 }, /* Indonesia */
	{ 100, 123, 123,   0,   32.58f,   54.27f, 432 }, /* Iran */
	{ 100, 117, 117,   0,   33.04f,   43.74f, 418 }, /* Iraq */
	{ 100, 107, 107,   0,   53.18f,   -8.14f, 272 }, /* Ireland */
	{ 100, 106, 106,   0,   31.46f,   35.00f, 425 }, /* Israel */
	{ 100, 119, 119,   0,   42.80f,   12.07f, 222 }, /* Italy */
	{ 100, 114, 114,   0,    7.55f,   -5.55f, 612 }, /* Ivory Coast */
	{ 100,  99,  99,   0,   18.16f,  -77.31f, 338 }, /* Jamaica */
	{  90, 127, 127,   0,   37.59f,  138.03f, 440 }, /* Japan */
	{  90, 127, 127,   0,   37.59f,  138.03f, 441 }, /* Japan */
	{ 100, 109, 109,   0,   31.25f,   36.77f, 416 }, /* Jordan */
	{ 100, 127, 127,   0,   48.16f,   67.29f, 401 }, /* Kazakhstan */
	{ 100, 118, 118,   0,    0.60f,   37.80f, 639 }, /* Kenya */
	{  95, 127, 127,   0,    1.87f, -157.36f, 545 }, /* Kiribati */
	{ 100, 112, 112,   0,   40.15f,  127.19f, 467 }, /* North Korea */
	{ 100, 113, 113,   0,   36.39f,  127.84f, 450 }, /* South Korea */
	{ 100,  98,  98,   0,   42.57f,   20.87f, 221 }, /* Kosovo */
	{ 100, 100, 100,   0,   29.33f,   47.59f, 419 }, /* Kuwait */
	{ 100, 114, 114,   0,   41.46f,   74.54f, 437 }, /* Kyrgyzstan */
	{ 100, 116, 116,   0,   18.21f,  103.89f, 457 }, /* Laos */
	{ 100, 107, 107,   0,   56.85f,   24.91f, 247 }, /* Latvia */
	{ 100,  99,  99,   0,   33.92f,   35.88f, 415 }, /* Lebanon */
	{ 100, 102, 102,   0,  -29.58f,   28.23f, 651 }, /* Lesotho */
	{ 100, 110, 110,   0,    6.45f,   -9.32f, 618 }, /* Liberia */
	{ 100, 122, 122,   0,   27.03f,   18.01f, 606 }, /* Libya */
	{ 100,  76,  76,   0,   47.14f,    9.54f, 295 }, /* Liechtenstein */
	{ 100, 106, 106,   0,   55.33f,   23.89f, 246 }, /* Lithuania */
	{ 100,  90,  90,   0,   49.77f,    6.07f, 270 }, /* Luxembourg */
	{ 100,  70,  70,   0,   22.22f,  113.51f, 455 }, /* Macau */
	{ 100, 100, 100,   0,   41.60f,   21.68f, 294 }, /* Macedonia */
	{ 100, 120, 120,   0,  -19.37f,   46.70f, 646 }, /* Madagascar */
	{ 100, 113, 113,   0,  -13.22f,   34.29f, 650 }, /* Malawi */
	{ 100, 123, 123,   0,    3.79f,  109.70f, 502 }, /* Malaysia */
	{ 100, 113, 113,   0,    3.73f,   73.46f, 472 }, /* Maldives */
	{ 100, 123, 123,   0,   17.35f,   -3.54f, 610 }, /* Mali */
	{ 100,  82,  82,   0,   35.92f,   14.41f, 278 }, /* Malta */
	{ 100, 117, 117,   0,    7.00f,  170.34f, 551 }, /* Marshall Islands */
	{ 100, 121, 121,   0,   20.26f,  -10.35f, 609 }, /* Mauritania */
	{ 100, 117, 117,   0,  -20.28f,   57.57f, 617 }, /* Mauritius */
	{  95, 127, 127,   0,   23.95f, -102.52f, 334 }, /* Mexico */
	{  40, 127, 127,   0,    7.45f,  153.24f, 550 }, /* Micronesia */
	{ 100, 105, 105,   0,   47.19f,   28.46f, 259 }, /* Moldova */
	{ 100,  65,  65,   0,   43.75f,    7.41f, 212 }, /* Monaco */
	{ 100, 124, 124,   0,   46.83f,  103.05f, 428 }, /* Mongolia */
	{ 100,  99,  99,   0,   42.79f,   19.24f, 297 }, /* Montenegro */
	{ 100,  73,  73,   0,   16.74f,  -62.19f, 354 }, /* Montserrat */
	{ 100, 123, 123,   0,   29.84f,   -8.46f, 604 }, /* Morocco */
	{ 100, 122, 122,   0,  -17.27f,   35.53f, 643 }, /* Mozambique */
	{ 100, 123, 123,   0,   21.19f,   96.49f, 414 }, /* Myanmar */
	{ 100,  67,  67,   0,   -0.52f,  166.93f, 536 }, /* Nauru */
	{ 100, 113, 113,   0,   28.25f,   83.92f, 429 }, /* Nepal */
	{  90, 103, 103,   0,   52.10f,    5.28f, 204 }, /* Netherlands */
	{ 100, 113, 113,   0,  -21.30f,  165.68f, 546 }, /* New Caledonia */
	{  70, 120, 127,   0,  -41.81f,  171.48f, 530 }, /* New Zealand */
	{ 100, 111, 111,   0,   12.85f,  -85.03f, 710 }, /* Nicaragua */
	{ 100, 122, 122,   0,   17.42f,    9.39f, 614 }, /* Niger */
	{ 100, 120, 120,   0,    9.59f,    8.09f, 621 }, /* Nigeria */
	{ 100,  77,  77,   0,  -19.05f, -169.87f, 555 }, /* Niue */
	{  85, 127, 117,   0,   68.75f,   15.35f, 242 }, /* Norway */
	{ 100, 117, 117,   0,   20.61f,   56.09f, 422 }, /* Oman */
	{ 100, 122, 122,   0,   29.95f,   69.34f, 410 }, /* Pakistan */
	{ 100, 110, 110,   0,    7.29f,  134.41f, 552 }, /* Palau */
	{ 100, 110, 110,   0,    8.52f,  -80.12f, 714 }, /* Panama */
	{ 100, 121, 121,   0,   -6.46f,  145.21f, 537 }, /* Papua New Guinea */
	{ 100, 116, 116,   0,  -23.23f,  -58.40f, 744 }, /* Paraguay */
	{ 100, 124, 124,   0,   -9.15f,  -74.38f, 716 }, /* Peru */
	{ 100, 122, 122,   0,   11.78f,  122.88f, 515 }, /* Philippines */
	{ 100, 113, 113,   0,   52.13f,   19.39f, 260 }, /* Poland */
	{ 100, 124, 124,   0,   39.60f,   -8.50f, 268 }, /* Portugal */
	{ 100, 101, 101,   0,   18.23f,  -66.47f, 330 }, /* Puerto Rico */
	{ 100,  97,  97,   0,   25.31f,   51.18f, 427 }, /* Qatar */
	{ 100, 113, 113,   0,   45.85f,   24.97f, 226 }, /* Romania */
	{  20, 127, 127,  90,   61.98f,   96.69f, 250 }, /* Russian Federation */
	{ 100, 101, 101,   0,   -1.99f,   29.92f, 635 }, /* Rwanda */
	{ 100,  72,  72,   0,  -12.40f,   -9.55f, 658 }, /* Saint Helena */
	{ 100,  83,  83,   0,   17.26f,  -62.69f, 356 }, /* Saint Kitts and Nevis */
	{ 100,  83,  83,   0,   13.89f,  -60.97f, 358 }, /* Saint Lucia */
	{ 100,  82,  82,   0,   46.92f,  -56.30f, 308 }, /* Saint Pierre and Miquelon */
	{ 100,  89,  89,   0,   13.22f,  -61.20f, 360 }, /* Saint Vincent and the Grenadines */
	{ 100,  95,  95,   0,  -13.75f, -172.16f, 549 }, /* Samoa */
	{ 100,  70,  70,   0,   43.94f,   12.46f, 292 }, /* San Marino */
	{ 100,  98,  98,   0,    0.44f,    6.72f, 626 }, /* Sao Tome and Principe */
	{ 100, 125, 125,   0,   24.12f,   44.54f, 420 }, /* Saudi Arabia */
	{ 100, 112, 112,   0,   14.37f,  -14.47f, 608 }, /* Senegal */
	{ 100, 107, 107,   0,   44.22f,   20.79f, 220 }, /* Serbia */
	{ 100, 117, 117,   0,   -4.66f,   55.48f, 633 }, /* Seychelles */
	{ 100, 106, 106,   0,    8.56f,  -11.79f, 619 }, /* Sierra Leone */
	{ 100,  82,  82,   0,    1.36f,  103.82f, 525 }, /* Singapore */
	{ 100, 106, 106,   0,   48.71f,   19.48f, 231 }, /* Slovakia */
	{ 100, 101, 101,   0,   46.12f,   14.80f, 293 }, /* Slovenia */
	{ 100, 119, 119,   0,   -8.92f,  159.63f, 540 }, /* Solomon Islands */
	{ 100, 121, 121,   0,    4.75f,   45.71f, 637 }, /* Somalia */
	{ 100, 127, 127,   0,  -29.00f,   25.08f, 655 }, /* South Africa */
	{ 100, 119, 119,   0,    7.31f,   30.25f, 659 }, /* South Sudan */
	{ 100, 125, 125,   0,   40.24f,   -3.65f, 214 }, /* Spain */
	{ 100, 107, 107,   0,    7.61f,   80.70f, 413 }, /* Sri Lanka */
	{ 100, 123, 123,   0,   15.99f,   29.94f, 634 }, /* Sudan */
	{ 100, 110, 110,   0,    4.13f,  -55.91f, 746 }, /* Suriname */
	{ 100,  96,  96,   0,  -26.56f,   31.48f, 653 }, /* Swaziland  */
	{ 100, 119, 119,   0,   62.78f,   16.75f, 240 }, /* Sweden */
	{ 100, 105, 105,   0,   46.80f,    8.21f, 228 }, /* Switzerland */
	{ 100, 112, 112,   0,   35.03f,   38.51f, 417 }, /* Syria */
	{ 100, 107, 107,   0,   23.75f,  120.95f, 466 }, /* Taiwan */
	{ 100, 112, 112,   0,   38.53f,   71.01f, 436 }, /* Tajikistan */
	{ 100, 118, 118,   0,   -6.28f,   34.81f, 640 }, /* Tanzania */
	{ 100, 121, 121,   0,   15.12f,  101.00f, 520 }, /* Thailand */
	{ 100, 109, 109,   0,    8.53f,    0.96f, 615 }, /* Togo */
	{ 100,  96,  96,   0,   -9.17f, -171.82f, 554 }, /* Tokelau */
	{ 100, 112, 112,   0,  -20.43f, -174.81f, 539 }, /* Tonga */
	{ 100,  98,  98,   0,   10.46f,  -61.27f, 374 }, /* Trinidad and Tobago */
	{ 100, 113, 113,   0,   34.12f,    9.55f, 605 }, /* Tunisia */
	{ 100, 120, 120,   0,   39.06f,   35.17f, 286 }, /* Turkey */
	{ 100, 118, 118,   0,   39.12f,   59.37f, 438 }, /* Turkmenistan */
	{ 100,  95,  95,   0,   21.83f,  -71.97f, 376 }, /* Turks and Caicos Islands */
	{ 100, 108, 108,   0,   -7.48f,  178.68f, 553 }, /* Tuvalu */
	{ 100, 113, 113,   0,    1.27f,   32.37f, 641 }, /* Uganda */
	{ 100, 119, 119,   0,   49.00f,   31.38f, 255 }, /* Ukraine */
	{ 100, 109, 109,   0,   24.35f,   53.94f, 424 }, /* United Arab Emirates */
	{ 100,  85,  85,   0,   24.47f,   54.37f, 430 }, /* United Arab Emirates (Abu Dhabi) */
	{ 100,  86,  86,   0,   25.07f,   55.17f, 431 }, /* United Arab Emirates (Dubai) */
	{ 100, 119, 119,   0,   54.12f,   -2.87f, 234 }, /* United Kingdom */
	{ 100, 119, 119,   0,   54.12f,   -2.87f, 235 }, /* United Kingdom */
	{  35, 127, 127,   0,   45.68f, -112.46f, 310 }, /* United States of America */
	{  35, 127, 127,   0,   45.68f, -112.46f, 311 }, /* United States of America */
	{  35, 127, 127,   0,   45.68f, -112.46f, 312 }, /* United States of America */
	{  35, 127, 127,   0,   45.68f, -112.46f, 313 }, /* United States of America */
	{  35, 127, 127,   0,   45.68f, -112.46f, 314 }, /* United States of America */
	{  35, 127, 127,   0,   45.68f, -112.46f, 315 }, /* United States of America */
	{  35, 127, 127,   0,   45.68f, -112.46f, 316 }, /* United States of America */
	{ 100,  89,  89,   0,   17.96f,  -64.80f, 332 }, /* United States Virgin Islands */
	{ 100, 111, 111,   0,  -32.80f,  -56.02f, 748 }, /* Uruguay */
	{ 100, 120, 120,   0,   41.76f,   63.14f, 434 }, /* Uzbekistan */
	{ 100, 113, 113,   0,  -16.23f,  167.69f, 541 }, /* Vanuatu */
	{ 100, 122, 122,   0,    7.12f,  -66.18f, 734 }, /* Venezuela */
	{ 100, 120, 120,   0,   16.65f,  106.30f, 452 }, /* Vietnam */
	{ 100, 100, 100,   0,  -13.89f, -177.35f, 543 }, /* Wallis and Futuna */
	{ 100, 118, 118,   0,   15.91f,   47.59f, 421 }, /* Yemen */
	{ 100, 119, 119,   0,  -13.46f,   27.77f, 645 }, /* Zambia */
	{ 100, 115, 115,   0,  -19.00f,   29.85f, 648 }, /* Zimbabwe */
};

const struct mcc_table *mcc_lookup(uint16_t mcc)
{
	for (int i = 0; i < ARRAY_SIZE(mcc_table); i++) {
		if (mcc_table[i].mcc == mcc) {
			return &mcc_table[i];
		}
	}

	return NULL;
}

int32_t lat_convert(float lat)
{
	return (int32_t)(lat * LAT_CONV);
}

int32_t lon_convert(float lon)
{
	return (int32_t)(lon * LON_CONV);
}
