#pragma once

const int WIDTH = 800;
const int WINDOW_WIDTH = 800;
const int HEIGHT = 600;
const int WINDOW_HEIGHT = 700;

const int GRID_TILE_SIZE = 20;

const int NB_OF_AGENTS = 50;

const float AGENT_DEFAULT_SIZE = 10;
const float AGENT_DEFAULT_SPEED = 100;

const int DISTANCE_SMELL = 75;
const int DISTANCE_SOUND = 150;

const int NB_OF_FOOD = 200;

const float FOOD_GRAB_PER_SEC = 0.5f;
const float FOOD_CONSUMPTION_PER_SEC = 0.1f;
const float FOOD_COST_TO_REPRODUCE = 2.0f;
const float KILL_FOOD = 0.5f;

const int BRAIN_SIZE = 70;

const int BRAIN_INPUT_LAYER_SIZE = 8;
const int BRAIN_HIDDEN_LAYER_SIZE = 10;
const int BRAIN_OUTPUT_LAYER_SIZE = 2;

const int SYNAPSE_PER_PERCEPTRON = 4;
const float SYNAPSE_WEIGHT_MIN = -2.f;
const float SYNAPSE_WEIGHT_MAX = 2.f;