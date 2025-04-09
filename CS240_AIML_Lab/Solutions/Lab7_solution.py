from sklearn.svm import SVC
from sklearn.metrics import accuracy_score, classification_report
from sklearn.model_selection import train_test_split
import numpy as np
import pandas as pd


def reduce_datset(X_train, y_train,X_test,y_test,train_size=10000,test_size=2000):
    # Ensure class balance using stratified sampling
    X_train_small, _, y_train_small, _ = train_test_split(X_train, y_train, train_size=train_size, stratify=y_train, random_state=42)
    X_test_small, _, y_test_small, _ = train_test_split(X_test, y_test, train_size=test_size, stratify=y_test, random_state=42)
    
    print(f"Reduced X_train shape: {X_train_small.shape}")
    print(f"Reduced y_train shape: {y_train_small.shape}")
    print(f"Reduced X_test shape: {X_test_small.shape}")
    print(f"Reduced y_test shape: {y_test_small.shape}")

    return X_train_small,y_train_small,X_test_small,y_test_small



def main():

    train_data=pd.read_csv(r"./mnist_train.csv")
    test_data=pd.read_csv(r"./mnist_test.csv")

    train_data=train_data.to_numpy()    # train_data shape: (60000, 785)
    m, n = train_data.shape
    print(m,n)
    test_data=test_data.to_numpy()      # test_data shape: (10000, 785)

    X_train=train_data[:,1:]            # X_train shape: (60000, 784)
    y_train=train_data[:,0]             # y_train shape: (60000,)
    X_test=test_data[:,1:]              # X_test shape: (10000, 784)
    y_test=test_data[:,0]               # y_test shape: (10000,)

    
    X_train,y_train,X_test,y_test = reduce_datset(X_train,y_train,X_test,y_test)  # For making inference faster.
    
    X_train = X_train / 255.0 
    X_test =X_test / 255.0


    # Train and evaluate a classifier for each digit (0-9)
    for digit in range(10):
        print(f"Training classifier for digit {digit} vs not-{digit}")
        
        # Convert labels to binary: 1 if the digit matches, else 0
        y_train_binary = np.where(y_train == digit, 1, 0)
        y_test_binary = np.where(y_test == digit, 1, 0)
        
        # Create and train the SVM model
        svm_model = SVC(kernel='linear', C=1.0)
        svm_model.fit(X_train, y_train_binary)
        
        # Make predictions
        y_pred = svm_model.predict(X_test)
        
        # Evaluate performance
        print(classification_report(y_test_binary, y_pred))



if __name__ == "__main__":
    main()