import streamlit as st
import datetime
import locale

def calculer_pi(digits):
    # bibliotheque decimales de PI
    from mpmath import mp
    mp.dps = digits + 2 
    pi = str(mp.pi)[2:]
    return pi

def rechercher_date_dans_pi(chiffres_pi, date_naissance):
    # Recherche
    return date_naissance in chiffres_pi

def obtenir_jour_semaine(date_naissance):
    annee, mois, jour = int(date_naissance[:4]), int(date_naissance[4:6]), int(date_naissance[6:8])
    jour_semaine = datetime.date(annee, mois, jour).strftime('%A')
    return jour_semaine

def somme_premieres_decimales(pi_digits, n):
    return sum(int(pi_digits[i]) for i in range(n))

def main():
    # Changer la locale en francais
    locale.setlocale(locale.LC_ALL, 'fr_FR')

    # Calcul sur 1 million de decimales
    chiffres_pi = calculer_pi(1000000)

    # Titre
    st.title("Recherche de date de naissance dans les decimales de PI")

    # Saisie date format ISO
    date_naissance = st.text_input("Entrez votre date de naissance (format: AAAAMMJJ)")

    if date_naissance and len(date_naissance) == 8:
        # Verification
        trouve = rechercher_date_dans_pi(chiffres_pi, date_naissance)

        # Resultat
        if trouve:
            st.success(f"La date de naissance {date_naissance} a ete trouvee !")
            jour_semaine = obtenir_jour_semaine(date_naissance)
            st.info(f"Cette date correspond a un(e) {jour_semaine.capitalize()}.")

    # Champs texte question 3
    st.subheader("Somme des 20 premieres decimales de PI")
    n1 = 20
    somme_n1 = somme_premieres_decimales(chiffres_pi, n1)
    st.text(f"La somme des {n1} premieres decimales de PI est : {somme_n1}")

    # Nouveaux champs texte pour la somme des 12^2 premieres decimales de PI
    st.subheader("Somme des 12^2 premieres decimales de PI")
    n2 = 12**2
    somme_n2 = somme_premieres_decimales(chiffres_pi, n2)
    st.text(f"La somme des {n2} premieres decimales de PI est : {somme_n2}")
    
    # Integration video
    st.subheader("Video : La somme des entiers naturels egale a -1/12?")
    st.video("https://www.youtube.com/watch?v=oaZKaV-oZ-g")

if __name__ == "__main__":
    main()