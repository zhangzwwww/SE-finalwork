import mlModel
import numpy as np
import matplotlib.pyplot as plt
from tqdm import tqdm # Displays a progress bar

import torch
from torch import nn
from torch import optim
import torch.nn.functional as F
from torchvision import datasets, transforms
from torch.utils.data import Dataset, Subset, DataLoader, random_split


# Load the dataset and train, val, test splits
# TODO: currently only experiment with FashionMNIST. Need to process your own dataset
print("Loading datasets...")
FASHION_transform = transforms.Compose([
    transforms.ToTensor(), # Transform from [0,255] uint8 to [0,1] float
    transforms.Normalize([0.2859], [0.3530]) # Normalize to zero mean and unit variance
])
FASHION_trainval = datasets.FashionMNIST('.', download=True, train=True, transform=FASHION_transform)
FASHION_train = Subset(FASHION_trainval, range(50000))
FASHION_val = Subset(FASHION_trainval, range(50000,60000))
FASHION_test = datasets.FashionMNIST('.', download=True, train=False, transform=FASHION_transform)
print("Done!")

# Create dataloaders
# TODO: Adjust your own batch size
trainloader = DataLoader(FASHION_train, batch_size=64, shuffle=True)
valloader = DataLoader(FASHION_val, batch_size=64, shuffle=True)
testloader = DataLoader(FASHION_test, batch_size=64, shuffle=True)

device = "cuda" if torch.cuda.is_available() else "cpu" # Configure device
model = mlModel.Network().to(device)
# TODO: choose your loss function
criterion = nn.CrossEntropyLoss()

# TODO: adjust optimizer, learning rate, weight decay according to your need
optimizer = optim.Adam(model.parameters(), lr=1e-3, weight_decay=1e-4)

# TODO: choose an appropriate number of epoch
num_epoch = 10


def train(model, loader, num_epoch = 10): # Train the model
    print("Start training...")
    model.train() # Set the model to training mode
    train_loss = []
    valid_loss = []
    for i in range(num_epoch):
        running_loss = []
        for batch, label in tqdm(loader):
            batch = batch.to(device)
            label = label.to(device)
            optimizer.zero_grad() # Clear gradients from the previous iteration
            pred = model(batch) # This will call Network.forward() that you implement
            loss = criterion(pred, label) # Calculate the loss
            running_loss.append(loss.item())
            loss.backward() # Backprop gradients to all tensors in the network
            optimizer.step() # Update trainable weights
        print("Epoch {} loss:{}".format(i+1,np.mean(running_loss))) # Print the average loss for this epoch
        train_loss.append(np.mean(running_loss))
        _, val_loss = evaluate(model, valloader)
        valid_loss.append(val_loss)
    print("Done!")
    # TODO: If needed, you can use following lines to print loss history
    x_axis = list(range(len(train_loss)))
    plt.plot(x_axis, train_loss)
    plt.plot(x_axis, valid_loss)
    plt.legend(['train-loss', 'valid-loss'])
    plt.xlabel("epoch number")
    plt.ylabel("Loss")
    plt.title('Loss for train and validation')
    plt.savefig('Loss_history.png')


def evaluate(model, loader): # Evaluate accuracy on validation / test set
    model.eval() # Set the model to evaluation mode
    correct = 0
    with torch.no_grad(): # Do not calculate grident to speed up computation
        running_loss = []
        for batch, label in tqdm(loader):
            batch = batch.to(device)
            label = label.to(device)
            pred = model(batch)
            loss = criterion(pred, label) # Calculate the loss
            running_loss.append(loss)
            correct += (torch.argmax(pred, dim=1) == label).sum().item()
    acc = correct/len(loader.dataset)
    print("Evaluation accuracy: {}".format(acc))
    return acc, np.mean(running_loss)


# save the parameters after training
param_save_path = "./model_param/model_states.pth"
train(model, trainloader, num_epoch)
torch.save(obj=model.state_dict(), f=param_save_path)

# evaluate the model
print("Evaluate on validation set...")
evaluate(model, valloader)
print("Evaluate on test set")
evaluate(model, testloader)