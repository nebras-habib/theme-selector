<?php

use Illuminate\Support\Facades\Route;
use App\Http\Controllers\WelcomeController;
use App\Http\Controllers\Dashboard\{
    HomeController,
    ColorController,
    OPCUAController
};

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', [WelcomeController::class, 'show'])->name('welcome');

Route::group([
    'prefix' => '/dashboard',
    'middleware' => ['auth'],
    'as' => 'dashboard.'
], function () {
    Route::get('/home', [HomeController::class, 'index'])->name('home');
    Route::get('/colors', [ColorController::class, 'create'])->name('colors.create');
    Route::post('/colors', [ColorController::class, 'store'])->name('colors.store');
    Route::get('/opc-ua', [OPCUAController::class, 'index'])->name('opcua.index');
    Route::get('/opc-ua/pumps/{pump}', [OPCUAController::class, 'show'])->name('opcua.pump.show');
    Route::post('/opc-ua/pumps/{pump}/{method}', [OPCUAController::class, 'callMethod'])->name('opcua.pump.callMethod');
});

require __DIR__ . '/auth.php';
