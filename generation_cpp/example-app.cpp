#include <torch/script.h>
#include <torch/torch.h>
#include <iostream>
#include <memory>

int main(int argc, const char* argv[]) {
    // load model
    std::shared_ptr<torch::jit::script::Module> module = torch::jit::load(argv[1]);
    int z_size = 100;
    int h_size = 5;
    int batch_size = 200;
    std::cout << module->forward(std::vector<torch::jit::IValue>({torch::rand({batch_size, z_size}), torch::rand({batch_size, h_size})})) << '\n';
    // output : Tensor size {200, 1, 32, 32}
}
