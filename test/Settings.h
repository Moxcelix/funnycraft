#pragma once
#include<iostream>
const int render_chunk_throughput = 2; // количество чанков на первичной отрисовке
const int update_chunk_throughput = 2; // количетсво чанкоы на обновление
const int create_chunk_throughput = 1; // количетсво чанкоы на создание
const std::string save_folder = "saves/worlds/"; // директория сохранения